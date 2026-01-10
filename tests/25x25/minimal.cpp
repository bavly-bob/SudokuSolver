#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(25);
    bool ok = board.loadFromFile("boards/25x25/minimal.txt");
    board.print();
    assert(ok && "Failed to load boards/25x25/minimal.txt");

    bool solved = board.solve();
    board.print();
    assert(solved);

    std::cout << "[OK] minimal board test passed\n";
    return 0;
}