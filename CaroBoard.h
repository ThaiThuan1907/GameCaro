#pragma once
#include <SDL.h>
#include <vector>
#include "Constants.h"

// Nhóm 5: Não Bộ & Luật Chơi [cite: 147-151]
class CaroBoard {
public:
    CaroBoard();
    ~CaroBoard();

    void Init();
    void Reset();
    
    // Logic đánh cờ
    bool PlacePiece(int row, int col, int player);
    bool CheckWin(int row, int col);
    
    // Hỗ trợ WASD (Con trỏ ảo)
    void MoveCursor(int dr, int dc);
    void GetCursorPos(int& r, int& c);

    int GetPiece(int r, int c) { return board[r][c]; }

private:
    int board[BOARD_ROWS][BOARD_COLS];
    int cursorRow = 0;
    int cursorCol = 0;
};
