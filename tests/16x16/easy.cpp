#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(16);
    bool ok = board.loadFromFile("boards/16x16/easy.txt");
    std::cout << "Loaded board: " << std::endl;
    board.print();
    assert(ok && "Failed to load boards/16x16/easy.txt");

    bool solved = board.propagateAll();
    std::cout << "Solved with propagation board: " << std::endl;
    board.print();
    assert(solved);

    std::cout << "[OK] Easy board test passed\n";
    return 0;
}