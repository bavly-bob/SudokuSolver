#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(9);
    bool ok = board.loadFromFile("boards/9x9/unsolvable.txt");
    assert(ok && "Failed to load boards/9x9/unsolvable.txt");

    bool solved = board.solve();
    assert(!solved);

    std::cout << "[OK] Unsolvable board test passed\n";
    return 0;
}
