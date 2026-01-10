import React, { useState } from "react";
import { Shuffle, Copy, Check } from "lucide-react";
import "./App.css";

const SudokuGenerator = () => {
  const [puzzle, setPuzzle] = useState("");
  const [solution, setSolution] = useState("");
  const [generating, setGenerating] = useState(false);
  const [copied, setCopied] = useState(false);
  const [size, setSize] = useState(9);
  const [difficulty, setDifficulty] = useState("medium");

  const getBoxSize = (n) => {
    if (n === 9) return 3;
    if (n === 16) return 4;
    if (n === 25) return 5;
    return 3;
  };

  const getClueCount = (n, diff) => {
    const total = n * n;
    const percentages = {
      easy: 0.55,
      medium: 0.40,
      hard: 0.30,
    };
    return Math.floor(total * percentages[diff]);
  };

  const isValid = (board, row, col, num, n) => {
    const boxSize = getBoxSize(n);

    for (let x = 0; x < n; x++) {
      if (board[row][x] === num) return false;
      if (board[x][col] === num) return false;
    }

    const startRow = row - (row % boxSize);
    const startCol = col - (col % boxSize);
    for (let i = 0; i < boxSize; i++) {
      for (let j = 0; j < boxSize; j++) {
        if (board[startRow + i][startCol + j] === num) return false;
      }
    }

    return true;
  };

  // Backtracking solver ONLY to generate a full valid board
  const solveSudoku = (board, n) => {
    for (let row = 0; row < n; row++) {
      for (let col = 0; col < n; col++) {
        if (board[row][col] === 0) {
          const numbers = [...Array(n)].map((_, i) => i + 1);
          for (let i = numbers.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [numbers[i], numbers[j]] = [numbers[j], numbers[i]];
          }

          for (const num of numbers) {
            if (isValid(board, row, col, num, n)) {
              board[row][col] = num;
              if (solveSudoku(board, n)) return true;
              board[row][col] = 0;
            }
          }
          return false;
        }
      }
    }
    return true;
  };

  const generatePuzzle = () => {
    setGenerating(true);
    setCopied(false);

    setTimeout(() => {
      const n = size;
      const boxSize = getBoxSize(n);

      // Empty board
      const board = Array.from({ length: n }, () => Array(n).fill(0));

      // Fill diagonal boxes
      const numBoxes = n / boxSize;
      for (let box = 0; box < numBoxes; box++) {
        const nums = [...Array(n)].map((_, i) => i + 1);
        for (let i = nums.length - 1; i > 0; i--) {
          const j = Math.floor(Math.random() * (i + 1));
          [nums[i], nums[j]] = [nums[j], nums[i]];
        }

        let idx = 0;
        for (let i = 0; i < boxSize; i++) {
          for (let j = 0; j < boxSize; j++) {
            board[box * boxSize + i][box * boxSize + j] = nums[idx++];
          }
        }
      }

      // Solve to get full valid grid
      solveSudoku(board, n);

      const solvedBoard = board.map((row) => [...row]);

      // Remove cells (NO uniqueness checking)
      const cellsToKeep = getClueCount(n, difficulty);
      const positions = [];

      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) positions.push([i, j]);
      }

      for (let i = positions.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [positions[i], positions[j]] = [positions[j], positions[i]];
      }

      let remaining = n * n;
      for (let k = 0; k < positions.length && remaining > cellsToKeep; k++) {
        const [r, c] = positions[k];
        if (board[r][c] !== 0) {
          board[r][c] = 0;
          remaining--;
        }
      }

      const toChar = (num) => {
        if (num === 0) return ".";
        if (num <= 9) return String(num);
        return String.fromCharCode(65 + num - 10);
      };

      const puzzleStr = board.map((r) => r.map(toChar).join("")).join("");
      const solutionStr = solvedBoard.map((r) => r.map(toChar).join("")).join("");

      setPuzzle(puzzleStr);
      setSolution(solutionStr);
      setGenerating(false);
    }, 50);
  };

  const formatBoard = (str) => {
    if (!str) return [];
    const rows = [];
    for (let i = 0; i < size; i++) {
      rows.push(str.slice(i * size, (i + 1) * size).split(""));
    }
    return rows;
  };

  const copyToClipboard = () => {
    navigator.clipboard.writeText(puzzle);
    setCopied(true);
    setTimeout(() => setCopied(false), 1500);
  };

  const board = formatBoard(puzzle);
  const boxSize = getBoxSize(size);
  const cellClass =
    size === 25 ? "cell cell-25" : size === 16 ? "cell cell-16" : "cell cell-9";

  return (
    <div className="container">
      <h1 className="title">Sudoku Puzzle Generator</h1>

      <div className="controls card">
        <div className="row three-columns">
          <div>
            <label className="label">Board Size</label>
            <div className="btn-row">
              {[9, 16, 25].map((s) => (
                <button
                  key={s}
                  onClick={() => setSize(s)}
                  className={`btn ${size === s ? "btn-active" : ""}`}
                >
                  {s}×{s}
                </button>
              ))}
            </div>
          </div>

          <div>
            <label className="label">Difficulty</label>
            <div className="btn-row">
              {["easy", "medium", "hard"].map((d) => (
                <button
                  key={d}
                  onClick={() => setDifficulty(d)}
                  className={`btn ${difficulty === d ? `btn-${d}` : ""}`}
                >
                  {d}
                </button>
              ))}
            </div>
          </div>

          <div className="generate-area">
            <button
              onClick={generatePuzzle}
              disabled={generating}
              className="btn-generate"
            >
              <Shuffle size={18} />{" "}
              {generating ? "Generating..." : "Generate"}
            </button>
          </div>
        </div>

        <div className="info">
          <strong>Current:</strong> {size}×{size}{" "}
          {difficulty.charAt(0).toUpperCase() + difficulty.slice(1)}
          {puzzle &&
            ` • ${puzzle.split("").filter((c) => c !== ".").length} clues`}
        </div>
      </div>

      {puzzle && (
        <>
          <div className="card puzzle-card">
            <div className="puzzle-header">
              <h2>Puzzle</h2>
              <button className="btn-copy" onClick={copyToClipboard}>
                {copied ? <Check size={14} /> : <Copy size={14} />}{" "}
                {copied ? "Copied!" : "Copy String"}
              </button>
            </div>

            <div className="board-wrap">
              <div
                className="board"
                style={{
                  gridTemplateColumns: `repeat(${size}, var(--cell-size))`,
                }}
              >
                {board.map((row, i) =>
                  row.map((cell, j) => {
                    const thickRight =
                      j % boxSize === boxSize - 1 && j !== size - 1;
                    const thickBottom =
                      i % boxSize === boxSize - 1 && i !== size - 1;

                    return (
                      <div
                        key={`${i}-${j}`}
                        className={`${cellClass} ${
                          cell === "." ? "empty" : "filled"
                        }`}
                        style={{
                          borderRight: thickRight
                            ? "2px solid #222"
                            : "1px solid #ccc",
                          borderBottom: thickBottom
                            ? "2px solid #222"
                            : "1px solid #ccc",
                        }}
                      >
                        {cell}
                      </div>
                    );
                  })
                )}
              </div>
            </div>

            <div className="puzzle-string">{puzzle}</div>
          </div>

          <div className="card hint-card">
            <h3>For C++ Solver:</h3>
            <pre className="code-block">
{`SudokuBoard board(${size});
board.loadFromString("${puzzle}");

if (board.solve()) {
    board.print();
} else {
    std::cout << "No solution found!\\n";
}`}
            </pre>
          </div>
        </>
      )}

      <div className="card legend">
        <h3>Legend:</h3>
        <div>
          <strong>9×9:</strong> Values 1-9<br />
          <strong>16×16:</strong> Values 1-9, A-G<br />
          <strong>25×25:</strong> Values 1-9, A-P<br />
          Empty cells = dots (.)
        </div>
      </div>
    </div>
  );
};

export default SudokuGenerator;
