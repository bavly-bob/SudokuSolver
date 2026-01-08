#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;
    board.loadFromFile("boards/unsolvable.txt");

    bool solved = board.solve();
    assert(!solved);

    std::cout << "[OK] Unsolvable board test passed\n";
    return 0;
}
