#include <iostream>
#include "Sudoku.h"
#include <chrono>
#include <string>
int main() {

    auto start = std::chrono::high_resolution_clock::now();

    Sudoku sudoku = Sudoku();

    for (int i = 1; i < 16; i++) {
        std::string file = std::string("tests/")+std::to_string(i)+std::string(".txt");
        std::cout << file << std::endl;
        sudoku.solve(file);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    std::cout << "Average microseconds elapsed per puzzle: " << duration.count()/15 << std::endl;
}