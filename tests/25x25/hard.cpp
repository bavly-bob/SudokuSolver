#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(25);
    bool ok = board.loadFromFile("boards/25x25/hard.txt");
    board.print();
    assert(ok && "Failed to load boards/25x25/hard.txt");

    bool solved = board.solve();
    board.print();
    assert(solved);

    std::cout << "[OK] hard board test passed\n";
    return 0;
}