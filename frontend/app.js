// app.js - Cleaned & optimized Sudoku frontend

const API_URL = "http://localhost:8080/solve";

class SudokuApp {
  constructor() {
    this.size = 9;
    this.boxSize = 3;
    this.board = [];
    this.originalBoard = [];

    this.initElements();
    this.bindEvents();
    this.createBoard();
  }

  initElements() {
    this.boardEl = document.getElementById("board");
    this.loadingEl = document.getElementById("loading");
    this.resultEl = document.getElementById("result");
    this.sizeButtons = document.querySelectorAll(".size-btn");
    this.solveBtn = document.getElementById("solveBtn");
    this.clearBtn = document.getElementById("clearBtn");
  }

  bindEvents() {
    this.sizeButtons.forEach((btn) => {
      btn.addEventListener("click", () => {
        const newSize = parseInt(btn.dataset.size, 10);
        if (newSize !== this.size) {
          this.size = newSize;
          this.boxSize = Math.sqrt(newSize);
          this.updateSizeButtons();
          this.createBoard();
        }
      });
    });

    // Single listeners on parent - event delegation
    this.boardEl.addEventListener("input", (e) => this.onBoardInput(e));
    this.boardEl.addEventListener("keydown", (e) => this.onBoardKeydown(e));

    this.solveBtn.addEventListener("click", () => this.solve());
    this.clearBtn.addEventListener("click", () => this.resetBoard());
  }

  updateSizeButtons() {
    this.sizeButtons.forEach((btn) => {
      btn.classList.toggle("active", parseInt(btn.dataset.size, 10) === this.size);
    });
  }

  createBoard() {
    // Initialize model
    this.board = Array.from({ length: this.size }, () => Array(this.size).fill(0));
    this.originalBoard = [];

    // Prepare DOM container
    this.boardEl.innerHTML = "";
    this.boardEl.className = `board size-${this.size}`;
    this.boardEl.style.gridTemplateColumns = `repeat(${this.size}, var(--cell-size))`;

    // Build with fragment to reduce reflows
    const frag = document.createDocumentFragment();

    for (let r = 0; r < this.size; r++) {
      for (let c = 0; c < this.size; c++) {
        frag.appendChild(this.createCell(r, c));
      }
    }

    this.boardEl.appendChild(frag);
    this.hideResult();
  }

  createCell(row, col) {
    const cell = document.createElement("div");
    cell.className = "cell";

    if ((col + 1) % this.boxSize === 0 && col < this.size - 1) cell.classList.add("box-right");
    if ((row + 1) % this.boxSize === 0 && row < this.size - 1) cell.classList.add("box-bottom");

    const input = document.createElement("input");
    input.type = "text";
    input.setAttribute("autocomplete", "off");
    input.setAttribute("spellcheck", "false");
    input.dataset.row = row;
    input.dataset.col = col;
    input.maxLength = this.size > 9 ? 2 : 1; // 25x25 needs letters sometimes
    input.inputMode = this.size <= 9 ? "numeric" : "text";
    input.className = "cell-input";

    cell.appendChild(input);
    return cell;
  }

  onBoardInput(e) {
    const t = e.target;
    if (!t || t.tagName !== "INPUT") return;

    const row = Number(t.dataset.row);
    const col = Number(t.dataset.col);
    if (Number.isNaN(row) || Number.isNaN(col)) return;

    let value = (t.value || "").toUpperCase().trim();
    let num = 0;

    if (value === "") {
      num = 0;
    } else if (this.size <= 9) {
      // allow only 1-9
      if (/^[1-9]$/.test(value)) num = parseInt(value, 10);
      else {
        t.value = "";
        return;
      }
    } else {
      // allow digits or letters representing values >= 10
      if (/^[1-9]$/.test(value)) {
        num = parseInt(value, 10);
      } else if (/^[A-Z]$/.test(value)) {
        num = value.charCodeAt(0) - 65 + 10;
        if (num > this.size) {
          t.value = "";
          return;
        }
      } else {
        t.value = "";
        return;
      }
      // preserve the uppercase letter if applicable
      t.value = value;
    }

    this.board[row][col] = num;
  }

  onBoardKeydown(e) {
    const t = e.target;
    if (!t || t.tagName !== "INPUT") return;

    // Use dataset to compute target index and move focus accordingly
    const row = Number(t.dataset.row);
    const col = Number(t.dataset.col);

    if (Number.isNaN(row) || Number.isNaN(col)) return;

    const inputs = this.boardEl.querySelectorAll("input");
    const idx = row * this.size + col;

    if (e.key === "ArrowUp" && row > 0) {
      inputs[idx - this.size].focus();
      e.preventDefault();
    } else if (e.key === "ArrowDown" && row < this.size - 1) {
      inputs[idx + this.size].focus();
      e.preventDefault();
    } else if (e.key === "ArrowLeft" && col > 0) {
      inputs[idx - 1].focus();
      e.preventDefault();
    } else if (e.key === "ArrowRight" && col < this.size - 1) {
      inputs[idx + 1].focus();
      e.preventDefault();
    } else if (e.key === "Backspace" && t.value === "") {
      // move left on backspace if cell empty
      if (col > 0) inputs[idx - 1].focus();
    }
  }

  getBoardString() {
    let str = "";
    for (let r = 0; r < this.size; r++) {
      for (let c = 0; c < this.size; c++) {
        const val = this.board[r][c];
        if (val === 0) str += "0";
        else if (val <= 9) str += String(val);
        else str += String.fromCharCode(65 + val - 10);
      }
    }
    return str;
  }

  async solve() {
    // snapshot original
    this.originalBoard = this.board.map((row) => [...row]);
    const boardString = this.getBoardString();

    this.showLoading();
    this.hideResult();

    // lightweight debug telemetry (non-blocking)
    fetch("http://127.0.0.1:7242/ingest/2153bc9e-4200-425f-9995-e337f7032a4b", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        sessionId: "debug-session",
        location: "frontend/app.js:solve:beforeFetch",
        apiUrl: API_URL,
        size: this.size,
        boardStringLength: boardString.length,
        timestamp: Date.now(),
      }),
    }).catch(() => {});

    try {
      const response = await fetch(API_URL, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ size: this.size, board: boardString }),
      });

      // debug
      fetch("http://127.0.0.1:7242/ingest/2153bc9e-4200-425f-9995-e337f7032a4b", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          sessionId: "debug-session",
          location: "frontend/app.js:solve:afterFetch",
          ok: response.ok,
          status: response.status,
          timestamp: Date.now(),
        }),
      }).catch(() => {});

      const data = await response.json();

      // debug
      fetch("http://127.0.0.1:7242/ingest/2153bc9e-4200-425f-9995-e337f7032a4b", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          sessionId: "debug-session",
          location: "frontend/app.js:solve:afterJson",
          hasSuccess: "success" in data,
          hasBoard: "board" in data,
          timestamp: Date.now(),
        }),
      }).catch(() => {});

      this.hideLoading();

      if (data.success) {
        this.displaySolution(data.board, data.time_ms);
      } else {
        this.showResult("error", `⚠️ ${data.error}`);
      }
    } catch (err) {
      // debug
      fetch("http://127.0.0.1:7242/ingest/2153bc9e-4200-425f-9995-e337f7032a4b", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          sessionId: "debug-session",
          location: "frontend/app.js:solve:catch",
          errorName: err?.name,
          errorMessage: err?.message,
          timestamp: Date.now(),
        }),
      }).catch(() => {});

      this.hideLoading();
      this.showResult("error", "❌ Could not connect to server. Make sure the C++ server is running.");
      console.error(err);
    }
  }

  displaySolution(solution, timeMs) {
    const inputs = this.boardEl.querySelectorAll("input");
    const cells = this.boardEl.querySelectorAll(".cell");

    // If the solver returned a different length, abort and show error
    if (!solution || solution.length !== this.size * this.size) {
      this.showResult("error", "Solver returned invalid board size.");
      return;
    }

    const animate = this.size <= 16; // animate only for small/medium boards
    if (!animate) {
      // instant apply for large boards (25x25)
      for (let i = 0; i < solution.length; i++) {
        const char = solution[i];
        const val = char >= "1" && char <= "9"
          ? parseInt(char, 10)
          : char >= "A" && char <= "Z"
          ? char.charCodeAt(0) - 65 + 10
          : 0;

        const input = inputs[i];
        const cell = cells[i];
        input.value = val <= 9 ? String(val) : String.fromCharCode(65 + val - 10);

        if (this.originalBoard[Math.floor(i / this.size)][i % this.size] === 0) {
          cell.classList.add("solved");
        } else {
          cell.classList.add("fixed");
        }
      }

      this.showResult("success", `✅ Solved in ${Number(timeMs).toFixed(2)} ms`);
      return;
    }

    // animated fill for smaller boards
    let delay = 0;
    for (let i = 0; i < solution.length; i++) {
      const char = solution[i];
      const val = char >= "1" && char <= "9"
        ? parseInt(char, 10)
        : char >= "A" && char <= "Z"
        ? char.charCodeAt(0) - 65 + 10
        : 0;

      const input = inputs[i];
      const cell = cells[i];

      if (this.originalBoard[Math.floor(i / this.size)][i % this.size] === 0) {
        setTimeout(() => {
          input.value = val <= 9 ? String(val) : String.fromCharCode(65 + val - 10);
          cell.classList.add("solved");
        }, delay);
        delay += 10;
      } else {
        cell.classList.add("fixed");
      }
    }

    setTimeout(() => {
      this.showResult("success", `✅ Solved in ${Number(timeMs).toFixed(2)} ms`);
    }, delay);
  }

  // Reset values and classes without rebuilding the DOM
  resetBoard() {
    this.board = Array.from({ length: this.size }, () => Array(this.size).fill(0));
    const inputs = this.boardEl.querySelectorAll("input");
    const cells = this.boardEl.querySelectorAll(".cell");

    inputs.forEach((inp) => (inp.value = ""));
    cells.forEach((cell) => {
      cell.classList.remove("solved", "fixed");
    });

    this.originalBoard = [];
    this.hideResult();
  }

  showLoading() {
    this.loadingEl.classList.add("visible");
    this.solveBtn.disabled = true;
  }

  hideLoading() {
    this.loadingEl.classList.remove("visible");
    this.solveBtn.disabled = false;
  }

  showResult(type, msg) {
    this.resultEl.className = `result ${type}`;
    this.resultEl.textContent = msg;
  }

  hideResult() {
    this.resultEl.className = "result";
    this.resultEl.textContent = "";
  }
}

document.addEventListener("DOMContentLoaded", () => {
  new SudokuApp();
});
