#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;
    board.loadFromFile("boards/minimal.txt");
    board.print();

    assert(board.solve());
    board.print();
    
    assert(board.isSolved());

    std::cout << "[OK] Minimal solve test passed\n";
    return 0;
}