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

void SudokuBoard::print() const 
{
    for (const auto& row : grid) 
    {
        for (const auto& cell : row) 
        {
            if (cell.hasOnlyOnePossibility()) 
            {
                for (int num = 1; num <= N; ++num) 
                {
                    if (cell.isPossible(num)) 
                    {
                        std::cout << num << " ";
                        break;
                    }
                }
            } 
            else 
            {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool SudokuBoard::isSolved() const
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (!grid[i][j].hasOnlyOnePossibility())
                return false;
    return true;
}

bool SudokuBoard::propagateAll()
{
    bool changed;
    do
    {
        changed = false;

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (!grid[i][j].hasOnlyOnePossibility())
                    continue;

                int val = -1;
                for (int num = 1; num <= N; ++num)
                {
                    if (grid[i][j].isPossible(num))
                    {
                        val = num;
                        break;
                    }
                }

                // Eliminate from row
                for (int k = 0; k < N; ++k)
                {
                    if (k != j && grid[i][k].isPossible(val))
                    {
                        grid[i][k].removePossibility(val);
                        changed = true;
                    }
                }

                // Eliminate from column
                for (int k = 0; k < N; ++k)
                {
                    if (k != i && grid[k][j].isPossible(val))
                    {
                        grid[k][j].removePossibility(val);
                        changed = true;
                    }
                }

                // Eliminate from box
                int boxStartRow = (i / 3) * 3;
                int boxStartCol = (j / 3) * 3;

                for (int r = boxStartRow; r < boxStartRow + 3; ++r)
                    for (int c = boxStartCol; c < boxStartCol + 3; ++c)
                        if ((r != i || c != j) && grid[r][c].isPossible(val))
                        {
                            grid[r][c].removePossibility(val);
                            changed = true;
                        }
                
                // Check for contradictions
                for (int r = 0; r < N; ++r)
                    for (int c = 0; c < N; ++c)   
                        if (grid[r][c].possibilityCount() == 0)
                            return false;
            }
        }
    } while (changed);

    return true;
}


bool SudokuBoard::backtracking() 
{
    // Find the first empty cell
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            if (!grid[i][j].hasOnlyOnePossibility()) 
            {
                for (int num = 1; num <= N; ++num) 
                {
                    if (grid[i][j].isPossible(num)) 
                    {
                        // Try this number
                        SudokuBoard snapshot = *this; // Save current state
                        grid[i][j].setValue(num);
                        
                        if (!propagateAll()) // If propagation fails 
                        {
                            *this = snapshot;
                            continue; // Try next number
                        }
                        
                        if (backtracking()) return true;
                        *this = snapshot; // Restore state
                    }
                }
                return false; // No valid number found
            }
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