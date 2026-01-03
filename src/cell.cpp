#include "cell.h"
#include <stdexcept>

cell::cell(int boardSize) 
        : N(boardSize) , possibilities((1u << N) - 1) 
{

    if (N == 9 || N == 16 || N == 25)
        throw std::invalid_argument("Board size must be 9, 16, or 25.");
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