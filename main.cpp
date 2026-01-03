#include "sudoku.h"
#include <iostream>
int main() {
    SudokuBoard board(9);

    // Example puzzle (0 = empty)
    std::string puzzle = 
        "530070000"
        "600195000"
        "098000060"
        "800060003"
        "400803001"
        "700020006"
        "060000280"
        "000419005"
        "000080079";

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