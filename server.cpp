#include "include/httplib.h"
#include "include/sudoku.h"

#include <iostream>
#include <chrono>
#include <sstream>

// ---------------- JSON helpers ----------------

struct Request {
    int size = 9;
    std::string board;
};

bool parseRequest(const std::string& json, Request& out) {
    auto findInt = [&](const std::string& key, int& value) {
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return false;
        pos = json.find(":", pos);
        if (pos == std::string::npos) return false;
        value = std::stoi(json.substr(pos + 1));
        return true;
    };

    auto findString = [&](const std::string& key, std::string& value) {
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return false;
        pos = json.find("\"", pos + key.size() + 2);
        if (pos == std::string::npos) return false;
        auto end = json.find("\"", pos + 1);
        if (end == std::string::npos) return false;
        value = json.substr(pos + 1, end - pos - 1);
        return true;
    };

    findInt("size", out.size);
    return findString("board", out.board);
}

// ---------------- Helpers ----------------

std::string boardToString(SudokuBoard& board, int size) {
    std::string result;
    result.reserve(size * size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int val = board.getCell(i, j).getValue();
            if (val == 0) result += '0';
            else if (val <= 9) result += char('0' + val);
            else result += char('A' + val - 10);
        }
    }
    return result;
}

// ---------------- Main ----------------

int main() {
    httplib::Server svr;

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    svr.Options("/solve", [](auto&, auto& res) {
        res.status = 204;
    });

    svr.Get("/health", [](auto&, auto& res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/solve", [](const httplib::Request& req, httplib::Response& res) {
        auto start = std::chrono::high_resolution_clock::now();

        Request parsed;
        if (!parseRequest(req.body, parsed)) {
            res.status = 400;
            res.set_content(R"({"success":false,"error":"Invalid JSON"})",
                            "application/json");
            return;
        }

        if (parsed.size != 9 && parsed.size != 16 && parsed.size != 25) {
            res.status = 400;
            res.set_content(R"({"success":false,"error":"Invalid size"})",
                            "application/json");
            return;
        }

        if ((int)parsed.board.size() != parsed.size * parsed.size) {
            res.status = 400;
            res.set_content(R"({"success":false,"error":"Invalid board length"})",
                            "application/json");
            return;
        }

        SudokuBoard board(parsed.size);
        if (!board.loadFromString(parsed.board)) {
            res.status = 400;
            res.set_content(R"({"success":false,"error":"Invalid board format"})",
                            "application/json");
            return;
        }

        bool solved = board.solve();
        auto end = std::chrono::high_resolution_clock::now();
        double time_ms =
            std::chrono::duration<double, std::milli>(end - start).count();

        if (!solved) {
            res.status = 200;
            res.set_content(R"({"success":false,"error":"No solution"})",
                            "application/json");
            return;
        }

        std::ostringstream json;
        json << R"({"success":true,"board":")"
             << boardToString(board, parsed.size)
             << R"(","time_ms":)" << time_ms << "}";

        res.status = 200;
        res.set_content(json.str(), "application/json");
    });

    std::cout << "Sudoku Solver API running at http://localhost:8080/solve\n";
    std::cout << "Press Ctrl+C to stop.\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
