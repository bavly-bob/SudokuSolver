cmake_minimum_required(VERSION 3.20)

project(SudokuSolver
    VERSION 1.0
    LANGUAGES CXX
)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Executable ----
add_executable(server_new
    server.cpp
    src/sudoku.cpp
    src/cell.cpp
)

# ---- Include directories ----
target_include_directories(server_new PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# ---- Windows-specific settings ----
if (WIN32)
    target_compile_definitions(server_new PRIVATE
        _WIN32_WINNT=0x0A00
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

    target_link_libraries(server_new PRIVATE
        ws2_32
    )
endif()
