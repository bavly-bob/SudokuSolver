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
    std::vector<Change> log;
public:
    explicit SudokuBoard(int boardSize);
    
    int charToValue(char ch);
    bool loadFromString(const std::string& puzzle); // load puzzle from string
    bool loadFromFile(const std::string& filename); // load puzzle from file
    void print() const; // print the board to console
    void clear(); // clear the board
    bool isSolved() const; // check if the board is completely solved
    bool isConsistent() const; // check if the current board state is valid
    int boxSize() const; // get the size of the boxes (e.g., 3 for 9x9)
    
    // Cell accessor for API
    const cell& getCell(int row, int col) const { return grid[row][col]; }
    
    // logging functions
    bool removePossibilityLogged(int r, int c, int num);
    void assign(int r, int c, int num);
    void rollback(int checkpoint);
    bool removeAllLogged(int row, int col, int num);
    bool propagateAllLogged();
    bool backtrackingLogged(); // solve the puzzle using backtracking if needed
    
    // helper function for further elmination of the possibilities  
    bool removeCol(int col, int num);
    bool removeRow(int row, int num);
    bool removeBox(int row, int col, int num);
    bool advancedRemoveCol(int row, int col, int num);
    bool advancedRemoveRow(int row, int col, int num);
    bool removeAll(int row, int col, int num);
    bool advancedRemoveAll(int row, int col, int num);

    // same helper functions for propagation but with logging 
    bool removeColLogged(int col, int num);
    bool removeRowLogged(int row, int num);
    bool removeBoxLogged(int row, int col, int num);
    bool hiddenSingleRowLogged(int row);
    bool hiddenSingleColLogged(int col);
    bool hiddenSingleBoxLogged(int row, int col);
    bool advancedRemoveColLogged(int boxRow, int boxCol, int num);
    bool advancedRemoveRowLogged(int boxRow, int boxCol, int num);
    bool advancedRemoveAllLogged(int boxRow, int boxCol, int num);
    
    
    void removePossibilitiesAfterInit(); // setup for the propagation 

    // more advanced elminators
    bool hiddenSingleRow(int row);
    bool hiddenSingleCol(int col);
    bool hiddenSingleBox(int row, int col);
    bool backtracking();
    bool hasContradiction() const; // check if the board has a contradiction
    bool propagateAll(); // perform constraint propagation on the entire board
    bool solve(); // high-level solve function combining propagation and backtracking
};
