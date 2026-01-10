#include "cell.h"
#include <stdexcept>

cell::cell(int boardSize) 
        : N(boardSize) , possibilities((1u << N) - 1) , cellValue(0)
{
    if (N != 9 && N != 16 && N != 25)
        throw std::invalid_argument("Board size must be 9 for standard Sudoku."); // solve the standard 9x9 Sudoku only
}

bool cell::removePossibility(int num) 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in removePossibility.");
    
    if (cellValue != 0) 
        return false; // fixed cells should not lose possibilities

    if(!isPossible(num))
        return false; // if the number is not possible, return false as it will not change anything

    possibilities &= ~(1u << (num - 1));
    return true;
}

bool cell::isPossible(int num) const 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in isPossible.");
    return possibilities & (1u << (num - 1));
}

void cell::clear() 
{
    possibilities = (1u << N) - 1;
    cellValue = 0;
}

void cell::setValue(int num) 
{
    if (num < 1 || num > N)
        throw std::out_of_range("Number out of range in setValue.");
    possibilities = (1u << (num - 1));
    cellValue = num;
}

bool cell::hasOnlyOnePossibility() const 
{
    return __builtin_popcount(possibilities) == 1;
}

int cell::possibilityCount() const 
{
    return __builtin_popcount(possibilities);
}

int cell::getSinglePossibility() const
{
    // __builtin_ffs returns 1-indexed position of the first set bit
    // Since we store bit (num-1) for value num, this gives us the value directly
    return __builtin_ffs(possibilities);
}

void cell::restore(uint32_t oldPoss, int oldValue)
{
    possibilities = oldPoss;
    cellValue = oldValue;
}