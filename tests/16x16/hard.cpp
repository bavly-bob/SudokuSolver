#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(16);
    bool ok = board.loadFromFile("boards/16x16/hard.txt");
    board.print();
    assert(ok && "Failed to load boards/16x16/hard.txt");

    bool solved = board.solve();
    board.print();
    assert(solved);

    std::cout << "[OK] Hard board test passed\n";
    return 0;
}