#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;
    board.loadFromFile("boards/hard.txt");
    assert(board.loadFromFile("boards/hard.txt") && "Failed to load boards/hard.txt");
    board.print();

    assert(board.solve());
    board.print();

    assert(board.isSolved());

    std::cout << "[OK] Hard backtracking test passed\n";
    return 0;
}
