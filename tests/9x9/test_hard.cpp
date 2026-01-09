#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(9);
    bool ok = board.loadFromFile("boards/9x9/hard.txt");
    assert(ok && "Failed to load boards/9x9/hard.txt");
    board.print();

    assert(board.solve());
    board.print();

    assert(board.isSolved());

    std::cout << "[OK] Hard backtracking test passed\n";
    return 0;
}
