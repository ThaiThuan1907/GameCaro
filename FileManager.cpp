#include "FileManager.h"
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Lưu: Player|Time|Mode|Difficulty và ma trận 12x12
bool SaveGame(const string& filename, int board[12][12], int currentPlayer, int matchTime, int gameMode, int aiDifficulty) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // Dòng đầu tiên: lưu các thông số trạng thái
    file << currentPlayer << " " << matchTime << " " << gameMode << " " << aiDifficulty << "\n";

    // 12 dòng tiếp theo: lưu bàn cờ 12x12
    for (int r = 0; r < 12; r++) {
        for (int c = 0; c < 12; c++) {
            file << board[r][c] << (c == 11 ? "" : " ");
        }
        file << "\n";
    }

    file.close();
    return true;
}

// Đọc: Player|Time|Mode|Difficulty và ma trận 12x12
bool LoadGame(const string& filename, int board[12][12], int& currentPlayer, int& matchTime, int& gameMode, int& aiDifficulty) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    // Đọc dòng trạng thái
    file >> currentPlayer >> matchTime >> gameMode >> aiDifficulty;

    // Đọc bàn cờ 12x12
    for (int r = 0; r < 12; r++) {
        for (int c = 0; c < 12; c++) {
            file >> board[r][c];
        }
    }

    file.close();
    return true;
}

// Liệt kê tất cả file .sav trong thư mục hiện tại
vector<string> ListSaveFiles() {
    vector<string> files;
    for (auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".sav")
            files.push_back(entry.path().filename().string());
    }
    return files;
}

// Xoá file theo tên
bool DeleteSaveFile(const string& filename) {
    return fs::remove(filename);
}

// Đổi tên file
bool RenameSaveFile(const string& oldName, const string& newName) {
    try {
        fs::rename(oldName, newName);
        return true;
    } catch (...) {
        return false;
    }
}
