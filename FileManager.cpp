#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Lưu: Player|Time|Mode|Difficulty và ma trận 12x12
bool FileManager::SaveGame(const std::string& filename, int board[12][12], int currentPlayer, int matchTime, int gameMode, int aiDifficulty) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // Dòng đầu tiên lưu các thông số trạng thái quan trọng [cite: 139]
    file << currentPlayer << " " << matchTime << " " << gameMode << " " << aiDifficulty << "\n";
    
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
bool FileManager::LoadGame(const std::string& filename, int board[12][12], int& currentPlayer, int& matchTime, int& gameMode, int& aiDifficulty) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    file >> currentPlayer >> matchTime >> gameMode >> aiDifficulty;
    
    for (int r = 0; r < 12; r++) {
        for (int c = 0; c < 12; c++) {
            file >> board[r][c];
        }
    }
    file.close();
    return true;
}

std::vector<std::string> FileManager::ListSaveFiles() {
    std::vector<std::string> files;
    for (auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".sav") files.push_back(entry.path().filename().string());
    }
    return files;
}

bool FileManager::DeleteFile(const std::string& filename) { return fs::remove(filename); }

bool FileManager::RenameFile(const std::string& oldName, const std::string& newName) {
    try { fs::rename(oldName, newName); return true; } catch (...) { return false; }
}
