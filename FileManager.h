#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Constants.h"

// Nhóm 4: Quản Lý Dữ Liệu
namespace FileManager {
    // Lưu thêm mode và difficulty
    bool SaveGame(const std::string& filename, int board[12][12], int currentPlayer, int matchTime, int gameMode, int aiDifficulty);
    bool LoadGame(const std::string& filename, int board[12][12], int& currentPlayer, int& matchTime, int& gameMode, int& aiDifficulty);
    
    std::vector<std::string> ListSaveFiles();
    bool DeleteFile(const std::string& filename);
    bool RenameFile(const std::string& oldName, const std::string& newName);
}
