#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;
    bool ok = board.loadFromFile("boards/easy.txt");
    assert(ok && "Failed to load boards/easy.txt");

    board.print();
    bool changed = board.propagateAll();
    std::cout << "After propagation:\n" << std::endl << changed << std::endl;
    board.print();
    changed = board.backtracking() || changed;
    board.print();
    assert(changed);
    assert(board.isSolved());

    std::cout << "[OK] Easy propagation test passed\n";
}
