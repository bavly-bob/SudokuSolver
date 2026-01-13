# SudokuSolver

A high-performance C++ Sudoku solver with a web-based frontend. This application allows users to solve Sudoku puzzles interactively, supporting boards of sizes 9x9, 16x16, and 25x25. It demonstrates efficient constraint propagation, bitmasking for memory optimization, and dynamic board updates.

## Features

- **High Performance**: Optimized C++ backend using bitmasks and constraint propagation.
- **Multiple Board Sizes**: Supports 9x9, 16x16, and 25x25 Sudoku grids.
- **Web Interface**: Simple HTML/JS frontend to interact with the solver.
- **API Support**: JSON-based REST API for solving puzzles.
- **Generator**: Includes a Sudoku generator (see `sudoku-generator/`).

## Repository Layout

```text
/
├── CMakeLists.txt       # Build configuration
├── server.cpp           # Main backend server implementation
├── frontend/            # Web interface (HTML, CSS, JS)
├── include/             # Header files
├── src/                 # Core solver implementation
├── tests/               # Unit tests for different difficulties/sizes
└── sudoku-generator/    # Sudoku puzzle generator
```

## Requirements

- **C++ Compiler**: GCC or Clang with C++17 support.
- **CMake**: Version 3.20 or later.
- **Make/Ninja**: Build tool.

## Build & Run (Backend)

The backend is a C++ server that solves puzzles sent via HTTP.

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/bavly-bob/SudokuSolver.git
    cd SudokuSolver
    ```

2.  **Create a build directory**:
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure and Build**:
    ```bash
    cmake ..
    cmake --build . -- -j$(nproc)
    ```
    *(On Windows, you might just run `cmake --build .` or use Visual Studio)*

4.  **Run the Server**:
    The executable is named `server_new`.
    ```bash
    ./server_new
    # Output: Sudoku Solver API running at http://localhost:8080/solve
    ```

## Frontend Setup

The frontend is a static web application located in the `frontend/` directory.

1.  **Start the Backend**: Ensure `server_new` is running on port 8080.
2.  **Open the Frontend**:
    You can open `frontend/index.html` directly in your browser.
    
    *Note: If you encounter CORS issues (common when opening `file://` URLs), serve the directory using a local web server:*
    
    **Using Python:**
    ```bash
    cd frontend
    python -m http.server 3000
    ```
    Then visit `http://localhost:3000`.

## Running Tests

Tests are standalone C++ files located in the `tests/` directory. You can compile and run them manually.

**Example: Run a 9x9 Easy Test**
```bash
# From the project root
g++ tests/9x9/test_easy.cpp src/*.cpp -Iinclude -o test_easy.exe
./test_easy.exe
```

## API Usage

The backend exposes a single endpoint:

- **POST** `/solve`
- **Body**: JSON
  ```json
  {
    "size": 9,
    "board": "530070000600195000098000060800060003400803001700020006060000280000419005000080079"
  }
  ```
  *(0 represents an empty cell)*

- **Response**: JSON
  ```json
  {
    "success": true,
    "board": "534678912672195348198342567859761423426853791713924856961537284287419635345286179",
    "time_ms": 12.23
  }
  ```

## C++ Design Notes

- **Constraint Propagation**: Automatically rules out impossible values to reduce the search space.
- **Bitmasking**: Uses integer bits to represent sets of numbers, allowing O(1) set operations.
- **Backtracking**: Efficient recursive search for valid solutions when deterministic logic reaches a limit.

## Contact

Maintained by Bavly Bob. Open an issue or PR on GitHub for questions or improvements.