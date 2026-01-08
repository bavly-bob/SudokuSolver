#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;
    board.loadFromFile("boards/medium.txt");
    assert(board.loadFromFile("boards/medium.txt") && "Failed to load boards/medium.txt");
    board.print();

    assert(board.solve());
    board.print();

    assert(board.isSolved());

    std::cout << "[OK] Medium solve test passed\n";
    return 0;
}
