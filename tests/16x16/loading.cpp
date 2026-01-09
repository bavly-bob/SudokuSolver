#include "sudoku.h"
#include <assert.h>
#include <iostream>

int main()
{
    SudokuBoard board(16);
    assert(board.loadFromFile("boards/16x16/easy.txt") && "Failed to load boards/16x16/easy.txt");
    assert(board.loadFromFile("boards/16x16/medium.txt") && "Failed to load boards/16x16/medium.txt");
    assert(board.loadFromFile("boards/16x16/hard.txt") && "Failed to load boards/16x16/hard.txt");
    assert(board.loadFromFile("boards/16x16/minimal.txt") && "Failed to load boards/16x16/minimal.txt");
    assert(board.loadFromFile("boards/16x16/unsolvable.txt") && "Failed to load boards/16x16/unsolvable.txt");

    std::cout << "[OK] Loading board test passed\n";
    return 0;
}