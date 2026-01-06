#include "sudoku.h"
#include <iostream>
int main() {
    SudokuBoard board(9);

    // Example puzzle (0 = empty)
    // This is a known solvable Sudoku puzzle that requires only propagation
    std::string puzzle = 
            "100000000"
            "020000000"
            "003000000"
            "000400000"
            "000050000"
            "000006000"
            "000000700"
            "000000080"
            "000000009";

    if (!board.loadFromString(puzzle)) {
        std::cerr << "Error loading puzzle!\n";
        return 1;
    }

    std::cout << "Original puzzle:\n";
    board.print();

    // First, run constraint propagation
    board.propagateAll();

    if (!board.isSolved()) {
        std::cout << "\nPropagation not enough, using backtracking...\n";
        if (!board.solveWithBacktracking()) {
            std::cout << "No solution found.\n";
            return 1;
        }
    }

    std::cout << "\nSolved puzzle:\n";
    board.print();
    return 0;
}