#pragma once
// SudokuBoard.h
// High-level board class that holds Cell objects and performs constraint propagation
// and auto-filling of single-candidate cells.

#include <vector>
#include <string>
#include <optional>
#include <cstdint>
#include "cell.h"

class SudokuBoard 
{
private:
    int N; // board size (e.g., 9 for 9x9)
    std::vector<std::vector<cell>> grid; // 2D grid of Cells
public:
    explicit SudokuBoard(int boardSize = 9);
    
    bool loadFromString(const std::string& puzzle); // load puzzle from string
    bool loadFromFile(const std::string& filename); // load puzzle from file
    void print() const; // print the board to console
    bool isSolved() const; // check if the board is completely solved
    bool propagateAll(); // perform constraint propagation on the entire board
    bool backtracking(); // solve the puzzle using backtracking if needed
    bool solve(); // high-level solve function combining propagation and backtracking
};
