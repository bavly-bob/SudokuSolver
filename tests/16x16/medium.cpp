#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(16);
    bool ok = board.loadFromFile("boards/16x16/medium.txt");
    assert(ok && "Failed to load boards/16x16/medium.txt");

    bool solved = board.solve();
    assert(solved);

    std::cout << "[OK] Medium board test passed\n";
    return 0;
}