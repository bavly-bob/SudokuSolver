#include "sudoku.h"
#include <iostream>

SudokuBoard::SudokuBoard(int boardSize) 
    : N(boardSize), grid(boardSize, std::vector<cell>(boardSize, cell(boardSize))) {}

bool SudokuBoard::loadFromString(const std::string& puzzle) 
{
    if (puzzle.length() != N * N) return false;
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            char ch = puzzle[i * N + j];
            if (ch >= '1' && ch <= '9') 
            {
                int num = ch - '0';
                grid[i][j].setValue(num);
            }
        }
    }
    return true;
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
}

bool SudokuBoard::isSolved() const 
{
    for (const auto& row : grid) 
    {
        for (const auto& cell : row) 
        {
            if (!cell.hasOnlyOnePossibility()) 
            {
                return false;
            }
        }
    }
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


bool SudokuBoard::solveWithBacktracking() 
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
                        propagateAll();
                        if (solveWithBacktracking()) 
                        {
                            return true;
                        }
                        *this = snapshot; // Restore state
                    }
                }
                return false; // No valid number found
            }
        }
    }
    return isSolved(); // If no empty cells, check if solved
}