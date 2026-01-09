#include "sudoku.h"
#include <cassert>
#include <iostream>

int main()
{
    SudokuBoard board(9);

    assert(board.loadFromFile("boards/9x9/easy.txt") && "Failed to load boards/easy.txt");
    assert(board.loadFromFile("boards/9x9/medium.txt") && "Failed to load boards/medium.txt");
    assert(board.loadFromFile("boards/9x9/hard.txt") && "Failed to load boards/hard.txt");
    assert(board.loadFromFile("boards/9x9/minimal.txt") && "Failed to load boards/minimal.txt");
    assert(board.loadFromFile("boards/9x9/unsolvable.txt") && "Failed to load boards/unsolvable.txt");

    std::cout << "[OK] Board loading tests passed\n";
    return 0;
}
