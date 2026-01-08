#include "sudoku.h"
#include <cassert>
#include <iostream>

void test_easy()
{
    SudokuBoard board;
    bool ok = board.loadFromFile("boards/easy.txt");
    assert(ok && "Failed to load boards/easy.txt");

    board.print();
    bool changed = board.propagateAll();
    board.print();
    changed = board.backtracking() || changed;
    board.print();
    assert(changed);
    assert(board.isSolved());

    std::cout << "[OK] Easy propagation test passed\n";
}
