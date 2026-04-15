#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Constants.h"

// Nhóm 4: Quản Lý Dữ Liệu [cite: 139-145]
class FileManager {
public:
    // Lưu thêm mode và difficulty [cite: 139]
    static bool SaveGame(const std::string& filename, int board[12][12], int currentPlayer, int matchTime, int gameMode, int aiDifficulty);
    static bool LoadGame(const std::string& filename, int board[12][12], int& currentPlayer, int& matchTime, int& gameMode, int& aiDifficulty);
    
    static std::vector<std::string> ListSaveFiles();
    static bool DeleteFile(const std::string& filename);
    static bool RenameFile(const std::string& oldName, const std::string& newName);
};
