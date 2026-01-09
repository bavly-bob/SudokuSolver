#include "sudoku.h"
#include <iostream>
#include <fstream>

SudokuBoard::SudokuBoard(int boardSize) 
    : N(boardSize), grid(boardSize, std::vector<cell>(boardSize, cell(boardSize))) {}

bool SudokuBoard::loadFromString(const std::string& puzzle) 
{
    if (puzzle.length() != N * N)
        return false;

    //  CLEAR BOARD FIRST
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            grid[i][j].clear();

    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            char ch = puzzle[i * N + j];
            if (ch >= '1' && ch <= '9') 
                grid[i][j].setValue(ch - '0');
        }
    }
    removePossibilitiesAfterInit();
    return true;
}


bool SudokuBoard::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) return false;
    
    std::string line;
    std::string puzzle;
    
    while (std::getline(file, line)) 
    {
        for (char ch : line) 
        {
            if ((ch >= '0' && ch <= '9') || ch == '.') 
            {
                puzzle += ch;
            }
        }
    }
    file.close();
    return loadFromString(puzzle);
}

void SudokuBoard::clear() 
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            grid[i][j].clear();
}

void SudokuBoard::print() const 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            std::cout << grid[i][j].getValue()<< " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int SudokuBoard::boxSize() const
{
    return (N == 9 ? 3 : N == 16 ? 4 : 5); // assuming only 9x9, 16x16, or 25x25 boards
}

bool SudokuBoard::isConsistent() const
{
    for (int i = 0; i < N; ++i)
    {
        std::vector<bool> row(N + 1, false), col(N + 1, false);

        for (int j = 0; j < N; ++j)
        {
            int rv = grid[i][j].getValue();
            int cv = grid[j][i].getValue();

            if (rv != 0)
            {
                if (row[rv]) return false;
                row[rv] = true;
            }

            if (cv != 0)
            {
                if (col[cv]) return false;
                col[cv] = true;
            }
        }
    }

    int root = boxSize();
    for (int br = 0; br < N; br += root)
        for (int bc = 0; bc < N; bc += root)
        {
            std::vector<bool> seen(N + 1, false);
            for (int r = 0; r < root; ++r)
                for (int c = 0; c < root; ++c)
                {
                    int v = grid[br + r][bc + c].getValue();
                    if (v != 0)
                    {
                        if (seen[v]) return false;
                        seen[v] = true;
                    }
                }
        }

    return true;
}

bool SudokuBoard::isSolved() const
{
    if(!isConsistent()) return false;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (grid[i][j].getValue() == 0)
                return false;
    return true;
}

void SudokuBoard::removePossibilitiesAfterInit()
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (grid[i][j].getValue() == 0)
                continue;

            int val = grid[i][j].getValue();
            // remove possibilities from row, column, and box
            removeAll(i, j, val);
        }
    }            
}

bool SudokuBoard::removeCol(int col, int num)
{
    bool changed = false;
    for (int row = 0; row < N; ++row)
        changed |= grid[row][col].removePossibility(num);
    return changed;
}

bool SudokuBoard::removeRow(int row, int num)
{
    bool changed = false;
    for (int col = 0; col < N; ++col)
        changed |= grid[row][col].removePossibility(num);
    return changed;
}

bool SudokuBoard::removeBox(int row, int col, int num)
{
    bool changed = false;
    int root = boxSize();
    int boxStartRow = (row / root) * root;
    int boxStartCol = (col / root) * root;

    for (int r = boxStartRow; r < boxStartRow + root; ++r)
        for (int c = boxStartCol; c < boxStartCol + root; ++c)
            if (!(r == row && c == col) && grid[r][c].isPossible(num))
                changed |= grid[r][c].removePossibility(num);

    return changed;
}


bool SudokuBoard::removeAll(int r, int c, int num)
{
    bool changed = false;
    changed |= removeCol(c, num);
    changed |= removeRow(r, num);
    changed |= removeBox(r, c, num);
    return changed;
}

bool SudokuBoard::advancedRemoveCol(int row, int col, int num)
{
    int root = boxSize();
    int boxStartRow = (row / root) * root;
    int boxStartCol = (col / root) * root;

    int restrictedCol = -1;

    // Scan the box
    for (int r = boxStartRow; r < boxStartRow + root; ++r)
        for (int c = boxStartCol; c < boxStartCol + root; ++c)
            if (grid[r][c].isPossible(num))
            {
                if (restrictedCol == -1) restrictedCol = c;
                else if (restrictedCol != c) return false; // not restricted to one column
            }
    
    // if no restricted column, return false
    if (restrictedCol == -1) return false;

    // Remove from the same column OUTSIDE the box
    bool changed = false;
    for (int r = 0; r < N; ++r)
    {
        if (r < boxStartRow || r >= boxStartRow + root)
            changed |= grid[r][restrictedCol].removePossibility(num);
    }

    return changed;
}


bool SudokuBoard::advancedRemoveRow(int row, int col, int num)
{
    int root = boxSize();
    int boxStartRow = (row / root) * root;
    int boxStartCol = (col / root) * root;

    int restrictedRow = -1;

    // Scan the box
    for (int r = boxStartRow; r < boxStartRow + root; ++r)
        for (int c = boxStartCol; c < boxStartCol + root; ++c)
            if (grid[r][c].isPossible(num))
            {
                if (restrictedRow == -1) restrictedRow = r;
                else if (restrictedRow != r) return false; // not restricted to one row
            }
    
    // if no restricted row, return false
    if (restrictedRow == -1) return false;
    
    // Remove from the same row OUTSIDE the box
    bool changed = false;
    for (int c = 0; c < N; ++c)
    {
        if (c < boxStartCol || c >= boxStartCol + root)
            changed |= grid[restrictedRow][c].removePossibility(num);
    }

    return changed;
}


bool SudokuBoard::advancedRemoveAll(int row, int col, int num)
{
    bool changed = false;
    changed |= advancedRemoveCol(row,col, num);
    changed |= advancedRemoveRow(row,col, num);
    return changed;
}

bool SudokuBoard::propagateAll()
{
    bool changed_advanced = false;
    bool changed_simple = false;
    do
    {
        changed_advanced = false;
        do
        {
            changed_simple = false;

            for (int i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    if (!grid[i][j].hasOnlyOnePossibility() || grid[i][j].getValue() != 0)
                        continue;

                    for(int num = 1; num <= N; ++num)
                    {
                        if (grid[i][j].isPossible(num) && grid[i][j].getValue() == 0)
                        {
                            grid[i][j].setValue(num); // set the number
                            break;
                        }
                    }
                    int val = grid[i][j].getValue();

                    // remove possibilities from row, column, and box
                    changed_simple = removeAll(i, j, val);
                    
                    // Check for contradictions
                    for (int r = 0; r < N; ++r)
                        for (int c = 0; c < N; ++c)   
                            if (grid[r][c].possibilityCount() == 0 && grid[r][c].getValue() == 0)
                                return false;
                }
            }
        } while (changed_simple);
        
        int root = boxSize();
        for(int i = 0; i < N; i += root)
            for(int j = 0; j < N; j += root)
                for(int k = 1; k <= N; ++k)
                    changed_advanced |= advancedRemoveAll(i, j, k);
    } while (changed_advanced);

    return true;
}


bool SudokuBoard::backtracking() 
{
    int bestR = -1, bestC = -1, bestCount = N + 1;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (grid[i][j].getValue() == 0)
            {
                int cnt = grid[i][j].possibilityCount();
                if (cnt < bestCount)
                {
                    bestCount = cnt;
                    bestR = i;
                    bestC = j;
                }
            }

    if (bestR == -1) return isSolved(); // No empty cells left
    for (int num = 1; num <= N; ++num) 
    {
        if (grid[bestR][bestC].isPossible(num)) 
        {
            // Try this number
            SudokuBoard snapshot = *this; // Save current state
            grid[bestR][bestC].setValue(num);
            removeAll(bestR, bestC, num);
            advancedRemoveAll(bestR, bestC, num);
                            
            if (!propagateAll()) // If propagation fails 
            {
                *this = snapshot;
                continue; // Try next number
            }
                        
            if (backtracking()) return true;
            *this = snapshot; // Restore state
        }
    }
    return isSolved(); // If no empty cells, check if solved
}

bool SudokuBoard::solve() 
{
    if (!propagateAll()) 
    {
        return false; // Contradiction found during propagation
    }
    if (isSolved()) 
    {
        return true; // Solved by propagation alone
    }
    return backtracking(); // Use backtracking if needed
}