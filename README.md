# SudokuSolver

A high-performance C++ Sudoku solver with a lightweight web frontend.
The project focuses on **algorithmic efficiency**, **clean solver architecture**, and **scalability** across large board sizes (9×9, 16×16, 25×25).

The backend is written in modern C++ (C++17) and exposes a simple HTTP API, while the frontend provides an interactive way to input puzzles and visualize solutions.

---

## Key Features

* **High-Performance Solver**
  Optimized C++ engine using constraint propagation, bitmasking, and heuristic backtracking.

* **Multiple Board Sizes**
  Fully generic solver supporting **9×9, 16×16, and 25×25** boards with alphanumeric encoding.

* **Web-Based Interface**
  Simple, dependency-free frontend built with **HTML, CSS, and JavaScript**.

* **REST API**
  JSON-based API for solving puzzles programmatically.

* **Sudoku Generator**
  Puzzle generator with multiple difficulty levels (see `sudoku-generator/`).

---

## Repository Layout

```text
/
├── CMakeLists.txt        # Build configuration
├── server.cpp            # HTTP server and API layer
├── frontend/             # Static frontend (HTML, CSS, JS)
├── include/              # Public headers
├── src/                  # Core solver implementation
├── tests/                # Unit tests (multiple sizes & difficulties)
└── sudoku-generator/     # Sudoku puzzle generator
```

---

## Requirements

* **C++ Compiler**: GCC or Clang with C++17 support
* **CMake**: ≥ 3.20
* **Build Tool**: Make or Ninja

---

## Build & Run (Backend)

1. **Clone the repository**

   ```bash
   git clone https://github.com/bavly-bob/SudokuSolver.git
   cd SudokuSolver
   ```

2. **Create build directory**

   ```bash
   mkdir build
   cd build
   ```

3. **Configure and build**

   ```bash
   cmake ..
   cmake --build . -- -j$(nproc)
   ```

4. **Run the server**

   ```bash
   ./server_new
   ```

   Output:

   ```
   Sudoku Solver API running at http://localhost:8080/solve
   ```

---

## Frontend Usage

1. Start the backend server on port `8080`
2. Open `frontend/index.html` in your browser

If your browser blocks requests due to CORS, serve the frontend locally:

```bash
cd frontend
python -m http.server 3000
```

Then visit:
`http://localhost:3000`

---

## API Usage

**Endpoint**

* `POST /solve`

**Request**

```json
{
  "size": 9,
  "board": "530070000600195000098000060800060003400803001700020006060000280000419005000080079"
}
```

**Response**

```json
{
  "success": true,
  "board": "534678912672195348198342567859761423426853791713924856961537284287419635345286179",
  "time_ms": 12.23
}
```

---

## Solver Architecture & Algorithms

### 1. Board Representation

* Each cell stores:

  * The current value (if fixed)
  * A **bitmask** representing possible candidates
* Bitmasks allow constant-time operations for:

  * Candidate elimination
  * Intersection
  * Validity checks

This design is memory-efficient and scales well to large boards (e.g., 25×25).

---

### 2. Constraint Propagation (Deterministic Phase)

Before any guessing, the solver repeatedly applies deterministic rules:

* **Naked Singles**
  Cells with exactly one remaining candidate are immediately assigned.

* **Hidden Singles**
  If a number can appear in only one cell within a row, column, or subgrid, it is forced.

* **Pointing Pairs / Intersections**
  Eliminates candidates from intersecting units when confined to a single row or column within a subgrid.

Constraint propagation dramatically reduces the search space before backtracking begins.

---

### 3. Heuristic-Driven Backtracking

When deterministic methods are exhausted:

* **MRV (Minimum Remaining Values)** heuristic selects the cell with the fewest candidates.
* Candidates are tried recursively with:

  * Logged assignments
  * Compact state snapshots
  * Efficient rollback on failure

This ensures:

* Minimal branching
* Fast recovery from invalid states
* Predictable performance even on large boards

---

### 4. Validation & Robustness

* Strict input validation (size, characters, board consistency)
* Early detection of contradictions
* Clear separation of concerns:

  * `Cell`
  * `Board`
  * `Solver`
  * `Generator`
  * `API`

---

## Testing

* Dedicated test cases for:

  * Multiple board sizes
  * Different difficulty levels
  * Edge cases and invalid inputs

**Example**

```bash
g++ tests/9x9/test_easy.cpp src/*.cpp -Iinclude -o test_easy
./test_easy
```

---

## Why This Project Matters

This solver is not just a Sudoku toy—it demonstrates:

* Practical use of **bitwise optimization**
* Clean **solver architecture**
* Real-world application of **constraint satisfaction problems (CSPs)**
* Bridging high-performance C++ with a usable web interface

---

## Contact

Maintained by **Bavly Bob**
GitHub issues and pull requests are welcome.

---

### Coach’s honest take

This README now **signals strong algorithmic thinking**, not just “I solved Sudoku.”
If you want, next we can:

* Add **complexity analysis** per phase
* Add a **performance comparison (9×9 vs 25×25)**
* Or tighten it further for **recruiters skimming in 30 seconds**

Just tell me the goal.
