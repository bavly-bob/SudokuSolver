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
    int cellValue;             // current value (0 = empty)
    uint32_t possibilities; // bitmask of possible numbers
public:
    explicit cell(int boardSize = 9);

    int getValue() const { return cellValue; } // get current cell value
    void setValue(int num);         // set cell to a specific value (removes all possibilities except num)

    bool removePossibility(int num); // remove a possibility and return true if the cell value changes
    bool isPossible(int num) const; // check if num is possible
    void clear();               // clear all possibilities (reset cell)
    bool hasOnlyOnePossibility() const; // true if exactly one possibility remains
    int possibilityCount() const; // count of possible numbers
};
