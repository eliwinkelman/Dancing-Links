//
// Created by Eli Winkelman on 9/5/19.
//

#include "Sudoku.h"
#include <iostream>
#include <fstream>

Sudoku::Sudoku(){

    Head = new Node();
    Head -> header = true;
    Head -> right = Head -> left = Head -> up = Head -> down = Head;

    if (setup()) {
        std::cout << "Initialized" << std::endl;
    }
    else {
        std::cout << "Failed to initialize dancing links." << std::endl;
    }
}

bool Sudoku::solve(std::string path) {
    std::ifstream file;
    solved = false;
    file.open(path);
    int v = 0;

    std::stack<Node*> coveredNodes;

    for (int c = 0; c < MATRIX_SIZE; c++) {
        for (int r = 0; r < MATRIX_SIZE; r++) {
            if (file.is_open()) {
                file >> v;
            } else {
                std::cout << "File Error";
                return false;
            }

            // make sure value is filled in
            if (v != 0) {
                v--;
                int row = (r * COL_OFFSET + c * MATRIX_SIZE + v);

                cover(matrix[ROW_OFFSET + r * MATRIX_SIZE + v][row]);
                cover(matrix[COL_OFFSET + c * MATRIX_SIZE + v][row]);
                cover(matrix[CELL_OFFSET + r * MATRIX_SIZE + c][row]);
                cover(matrix[BOX_OFFSET + ((r / ROW_BOX_DIVISOR + c / COL_BOX_DIVISOR * COL_BOX_DIVISOR) * MATRIX_SIZE +
                                           v)][row]);

                currentSolution.push(matrix[ROW_OFFSET + r * MATRIX_SIZE + v][row]);

                coveredNodes.push(matrix[ROW_OFFSET + r * MATRIX_SIZE + v][row]);
                coveredNodes.push(matrix[COL_OFFSET + c * MATRIX_SIZE + v][row]);
                coveredNodes.push(matrix[CELL_OFFSET + r * MATRIX_SIZE + c][row]);
                coveredNodes.push(matrix[BOX_OFFSET + ((r / ROW_BOX_DIVISOR + c / COL_BOX_DIVISOR * COL_BOX_DIVISOR) * MATRIX_SIZE +
                                                       v)][row]);

                // find the headers belonging to this value
            }
        }
    }

    solved = solve(0);
    print();

    /** prepare for doing this again */

    while(!coveredNodes.empty()) {
        uncover(coveredNodes.top());
        coveredNodes.pop();
    }

    while(!currentSolution.empty()) {
        currentSolution.pop();
    }

    return solved;
}

void Sudoku::AddHeader(Node *header) {
    if (header -> header) {
        AddHeader(header, Head);
    }
}

void Sudoku::AddHeader(Node *header, Node *right) {
    if (right -> right == Head && right != header) {
        // add the header
        //add column to the end of the column list
        right->right->left = header;
        header->right = right->right;
        header->left = right;
        right->right = header;

    }
    else {
        AddHeader(header, right -> right);
    }
}


void Sudoku::print() {

    int puzzle[MATRIX_SIZE][MATRIX_SIZE];

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            puzzle[i][j] = 0;
        }
    }

    std::stack<Node*> saveSolution;
    while (!currentSolution.empty()) {
        Node* fillIn = currentSolution.top();
        puzzle[fillIn->col][fillIn->row] = fillIn->val;
        currentSolution.pop();
        saveSolution.push(fillIn);
    }

    //print the puzzle matrix

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {

            std::cout << puzzle[i][j] << " ";

        }
        std::cout << std::endl;
    }

    // regenerate the current solution
    while(!saveSolution.empty()){
        currentSolution.push(saveSolution.top());
        saveSolution.pop();
    }
}

Node* Sudoku::findNextHeader() {
    Node* nextColumnNode = nullptr;
    int minimumRows = -1;

    for (Node* colNode = Head -> right; colNode != Head; colNode = colNode -> right) {
        if (minimumRows == -1 || colNode -> s < minimumRows) {
            minimumRows = colNode -> s;
            nextColumnNode = colNode;
        }
    }
    return nextColumnNode;
}

bool Sudoku::solve(int n) {

    if (Head == Head -> right) {
        std::cout << "Solved!" << std::endl;
        return true;
    }

    Node* colHeader = findNextHeader();

    Node* rowNode = nullptr;
    Node* colNode = nullptr;

    cover(colHeader);
    for(rowNode = colHeader -> down; rowNode != colHeader && !solved; rowNode = rowNode -> down) {
        currentSolution.push(rowNode);
        for(colNode = rowNode -> right; colNode != rowNode; colNode = colNode -> right) {
            cover(colNode);
        }
        solved = solve(n+1);
        if (!solved){
            currentSolution.pop();
        }
        for (colNode = rowNode -> left; colNode != rowNode; colNode = colNode -> left) {
            uncover(colNode);
        }
    }
    uncover(colHeader);
    return solved;
}


void Sudoku::cover(Node* colHeader) {
    // make sure this is actually a column header
    colHeader = colHeader -> columnHeader;
    if (colHeader == Head) {
        std::cout << "Head as column header. ABORT!" << std::endl;
    }

    // disconnect the column header
    colHeader -> right -> left = colHeader -> left;
    colHeader -> left -> right = colHeader -> right;

    Node *rowNode, *colNode = nullptr;

    for (rowNode = colHeader -> down; rowNode != colHeader; rowNode = rowNode -> down) {
        for (colNode = rowNode -> right; colNode != rowNode; colNode = colNode -> right) {
            colNode -> up -> down = colNode -> down;
            colNode -> down -> up = colNode -> up;
            colNode -> columnHeader -> s--;
        }
    }
}

void Sudoku::uncover(Node* colHeader) {
    colHeader= colHeader -> columnHeader;
    if (colHeader == Head) {
        std::cout << "Head as column header. ABORT!" << std::endl;
    }
    Node *rowNode, *colNode = nullptr;

    for (rowNode = colHeader -> up; rowNode != colHeader; rowNode = rowNode -> up) {

        for (colNode = rowNode-> left; colNode != rowNode; colNode = colNode -> left) {
            colNode -> up -> down = colNode;
            colNode -> down -> up = colNode;
            colNode -> columnHeader -> s++;
        }
    }

    colHeader -> right -> left = colHeader;
    colHeader -> left -> right = colHeader;
}

bool Sudoku::setup() {

    for (int i = 0; i < MAX_COLS; i++) {
        for (int j = 0; j < MAX_ROWS; j++) {
            matrix[i][j] = nullptr;
        }
    }

    Node* rowNode, *colNode, *cellNode, *cubeNode = nullptr;
    for (int r = 0; r < MATRIX_SIZE; r++){
        for (int c = 0; c < MATRIX_SIZE; c++) {
            for (int v = 0; v < MATRIX_SIZE; v++){
                int row = (r*COL_OFFSET+c*MATRIX_SIZE+v);
                rowNode = matrix[ROW_OFFSET+r*MATRIX_SIZE+v][row] = new Node(c, r, v+1);
                colNode = matrix[COL_OFFSET+c*MATRIX_SIZE+v][row] = new Node(c, r, v+1);
                cellNode = matrix[CELL_OFFSET+r*MATRIX_SIZE+c][row] = new Node(c, r, v+1);
                cubeNode = matrix[BOX_OFFSET+((r/ROW_BOX_DIVISOR + c/COL_BOX_DIVISOR * COL_BOX_DIVISOR)*MATRIX_SIZE+v)][row] = new Node(c, r, v+1);

                // link them
                rowNode -> right = colNode;
                colNode -> right = cellNode;
                cellNode -> right = cubeNode;
                cubeNode -> right = rowNode;

                rowNode -> left = cubeNode;
                cubeNode -> left = cellNode;
                cellNode -> left = colNode;
                colNode -> left = rowNode;

            }
        }
    }

    Node* colHeader = nullptr;
    rowNode = nullptr;

    for (int c = 0; c < MAX_COLS; c++) {
        /// create a new column header;
        colHeader = new Node();
        colHeader -> header = true;
        colHeader -> right = colHeader -> left = colHeader -> up = colHeader -> down = colHeader -> columnHeader = colHeader;
        colHeader -> s = 0;
        Node* lastRowNode = colHeader;
        for (int r = 0; r < MAX_ROWS; r++) {
            if (matrix[c][r] != nullptr) {
                // there is a row node here
                rowNode = matrix[c][r];
                // link it
                rowNode -> up = lastRowNode;
                rowNode -> down = colHeader;
                colHeader -> up = rowNode;
                lastRowNode -> down = rowNode;

                rowNode -> columnHeader = colHeader;
                lastRowNode = rowNode;

                (colHeader -> s)++;
            }
        }

        if (colHeader -> s == 0) {
            std::cout << "Error adding column!" << std::endl;
            return false;
        }
        // add the header
        AddHeader(colHeader);
    }

    return true;

}