#include "cell.h"
#include <stdexcept>

cell::cell(int boardSize) 
        : N(boardSize) , possibilities((1u << N) - 1) 
{

    if (N != 9)
        throw std::invalid_argument("Board size must be 9 for standard Sudoku."); // solve the standard 9x9 Sudoku only
}

void cell::removePossibility(int num) 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in removePossibility.");
    possibilities &= ~(1u << (num - 1));
}

bool cell::isPossible(int num) const 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in isPossible.");
    return possibilities & (1u << (num - 1));
}

void cell::setValue(int num) 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in setValue.");
    possibilities = (1u << (num - 1));
}

bool cell::hasOnlyOnePossibility() const 
{
    return __builtin_popcount(possibilities) == 1;
}

int cell::possibilityCount() const 
{
    return __builtin_popcount(possibilities);
}