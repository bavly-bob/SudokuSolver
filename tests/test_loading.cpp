#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board;

    assert(board.loadFromFile("boards/easy.txt") && "Failed to load boards/easy.txt");
    assert(board.loadFromFile("boards/medium.txt") && "Failed to load boards/medium.txt");
    assert(board.loadFromFile("boards/hard.txt") && "Failed to load boards/hard.txt");
    assert(board.loadFromFile("boards/minimal.txt") && "Failed to load boards/minimal.txt");
    assert(board.loadFromFile("boards/unsolvable.txt") && "Failed to load boards/unsolvable.txt");

    std::cout << "[OK] Board loading tests passed\n";
    return 0;
}
