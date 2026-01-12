#include "include/httplib.h"
#include "include/sudoku.h"

#include <iostream>
#include <chrono>
#include <sstream>


// Simple JSON parser for our specific format
struct Request {
    int size;
    std::string board;
    
    static Request parse(const std::string& json) {
        Request req;
        req.size = 9;  // default
        
        // Find "size": value
        size_t pos = json.find("\"size\"");
        if (pos != std::string::npos) {
            pos = json.find(":", pos);
            if (pos != std::string::npos) {
                req.size = std::stoi(json.substr(pos + 1));
            }
        }
        
        // Find "board": "value"
        pos = json.find("\"board\"");
        if (pos != std::string::npos) {
            pos = json.find("\"", pos + 7);
            if (pos != std::string::npos) {
                size_t end = json.find("\"", pos + 1);
                if (end != std::string::npos) {
                    req.board = json.substr(pos + 1, end - pos - 1);
                }
            }
        }
        
        return req;
    }
};

std::string boardToString(SudokuBoard& board, int size) {
    std::string result;
    result.reserve(size * size);
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int val = board.getCell(i, j).getValue();
            if (val == 0) {
                result += '0';
            } else if (val <= 9) {
                result += ('0' + val);
            } else {
                result += ('A' + val - 10);
            }
        }
    }
    return result;
}

int main() {
    httplib::Server svr;
    
    // Enable CORS for browser access
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    
    // Handle preflight OPTIONS request
    svr.Options("/solve", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });
    
    // Main solve endpoint
    svr.Post("/solve", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            Request parsed = Request::parse(req.body);
            
            // Validate size
            if (parsed.size != 9 && parsed.size != 16 && parsed.size != 25) {
                res.set_content(R"({"success":false,"error":"Invalid size. Use 9, 16, or 25."})", 
                               "application/json");
                return;
            }
            
            // Validate board length
            if ((int)parsed.board.length() != parsed.size * parsed.size) {
                res.set_content(R"({"success":false,"error":"Board length doesn't match size."})", 
                               "application/json");
                return;
            }
            
            SudokuBoard board(parsed.size);
            if (!board.loadFromString(parsed.board)) {
                res.set_content(R"({"success":false,"error":"Invalid board format."})", 
                               "application/json");
                return;
            }
            
            bool solved = board.solve();
            
            auto end = std::chrono::high_resolution_clock::now();
            double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
            
            if (solved) {
                std::string solvedBoard = boardToString(board, parsed.size);
                std::ostringstream json;
                json << R"({"success":true,"board":")" << solvedBoard 
                     << R"(","time_ms":)" << time_ms << "}";
                res.set_content(json.str(), "application/json");
            } else {
                res.set_content(R"({"success":false,"error":"No solution exists."})", 
                               "application/json");
            }
            
        } catch (const std::exception& e) {
            std::string error = R"({"success":false,"error":")" + std::string(e.what()) + "\"}";
            res.set_content(error, "application/json");
        }
    }); 

    std::cout << "Sudoku Solver API running at http://localhost:8080/solve" << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;
    
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}
