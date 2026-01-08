#include "sudoku.h"
#include <iostream>
int main() 
{
    SudokuBoard board(9);
    if (!board.loadFromFile("boards/easy.txt")) 
    {
        std::cerr << "Failed to load puzzle from file." << std::endl;
        return 1;
    }
    std::cout << "Initial Puzzle:" << std::endl;
    board.print();
    if(board.solve()) 
    {
        std::cout << "Solved Puzzle:" << std::endl;
        board.print();
    } 
    else 
    {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}