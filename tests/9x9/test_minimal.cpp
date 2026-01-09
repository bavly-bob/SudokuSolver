#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(9);
    bool ok = board.loadFromFile("boards/9x9/minimal.txt");
    assert(ok && "Failed to load boards/9x9/minimal.txt");
    board.print();

    assert(board.solve());
    board.print();
    
    assert(board.isSolved());

    std::cout << "[OK] Minimal solve test passed\n";
    return 0;
}