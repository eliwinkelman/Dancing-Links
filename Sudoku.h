//
// Created by Eli Winkelman on 9/5/19.
//

#ifndef DANCING_LINKS_SUDOKU_H
#define DANCING_LINKS_SUDOKU_H

#include <stack>
#include <string>

struct Node {
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    Node* columnHeader;

    bool header;
    int val;
    int col;
    int row;
    int s;

    Node() {
        up = down = left = right = columnHeader = nullptr;
        col = -1;
        row = -1;
        val = -1;
        s = -1;
        header = false;
    }

    Node(int c, int r, int v) : Node() {
        col = c;
        row = r;
        val = v;
        s = -1;
    }
};

class Sudoku {

    static constexpr int  ROW_OFFSET = 0;
    static constexpr int  COL_OFFSET = 81;
    static constexpr int CELL_OFFSET = 162;
    static constexpr int BOX_OFFSET = 243;

    //these ultimately define the proportions of our matrix
    static constexpr int MATRIX_SIZE = 9;
    static constexpr int  MAX_COLS = 324;
    static constexpr int MAX_ROWS = 729;

    static constexpr int COL_BOX_DIVISOR = 3; //number of boxes in a column
    static constexpr int ROW_BOX_DIVISOR = 3; //number of boxes in a row

    bool setup();
    void cover(Node* colNode);
    void uncover(Node* colNode);

    void AddHeader(Node* header);
    void AddHeader(Node* header, Node* right);

    Node* findNextHeader();

    bool solved = false;

    std::stack<Node*> currentSolution;

    Node* Head;

    Node* matrix[MAX_COLS][MAX_ROWS];

public:

    Sudoku();

    bool solve(std::string path);
    bool solve(int n);
    //debug helper
    void print();

};


#endif //DANCING_LINKS_SUDOKU_H
