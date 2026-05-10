#pragma once
#include <string>
#include <vector>
using namespace std;
// Nhóm 4: Quản Lý Dữ Liệu - Phong cách thủ tục (không OOP)

// Lưu trạng thái ván cờ ra file .sav
bool SaveGame(const string& filename, int board[12][12], int currentPlayer, int matchTime, int gameMode, int aiDifficulty);

// Đọc trạng thái ván cờ từ file .sav
bool LoadGame(const string& filename, int board[12][12], int& currentPlayer, int& matchTime, int& gameMode, int& aiDifficulty);

// Liệt kê tất cả file .sav trong thư mục hiện tại
vector<string> ListSaveFiles();

// Xoá file theo tên
bool DeleteSaveFile(const string& filename);

// Đổi tên file
bool RenameSaveFile(const string& oldName, const string& newName);
