#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(9);
    bool ok = board.loadFromFile("boards/9x9/easy.txt");
    assert(ok && "Failed to load boards/9x9/easy.txt");

    board.print();
    bool changed = board.propagateAll();
    std::cout << "After propagation:\n" << std::endl << changed << std::endl;
    board.print();
    changed |= board.backtracking();
    board.print();
    assert(changed);
    assert(board.isSolved());

    std::cout << "[OK] Easy propagation test passed\n";
}
