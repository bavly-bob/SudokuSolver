#pragma once
// Cell.h
// Represents a single Sudoku cell: current value (0 = empty) and a bitmask of possibilities.
// Uses a uint32_t mask where bit (num-1) set => number `num` is possible.

#include <cstdint>
#include <string>

class cell 
{
private:
    int N;                  // max number (board size)
    uint32_t possibilities; // bitmask of possible numbers
public:
    explicit cell(int boardSize = 9);

    void removePossibility(int num); // remove a possibility
    bool isPossible(int num) const; // check if num is possible
    void clear();               // clear all possibilities (reset cell)
    void setValue(int num);         // set cell to a specific value (removes all possibilities except num)
    bool hasOnlyOnePossibility() const; // true if exactly one possibility remains
    int possibilityCount() const; // count of possible numbers
};
