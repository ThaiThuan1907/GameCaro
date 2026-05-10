#pragma once
#include <SDL.h>
#include <vector>
#include "Constants.h"

// Nhóm 5: Não Bộ & Luật Chơi
struct BoardState {
    int board[BOARD_ROWS][BOARD_COLS] = { 0 };
    int cursorRow = 0;
    int cursorCol = 0;
    
    // Lưu tọa độ 5 quân thắng để vẽ highlight
    struct Coord { int r, c; };
    Coord winningCoords[5];
};

// Các hàm khởi tạo và reset
void InitBoardState(BoardState* state);
void ResetBoardState(BoardState* state);

// Logic đánh cờ
bool PlacePiece(BoardState* state, int row, int col, int player);
bool CheckWin(BoardState* state, int row, int col);
bool IsBoardFull(BoardState* state);

// Hỗ trợ điều khiển (Con trỏ ảo)
void MoveBoardCursor(BoardState* state, int dr, int dc);
int GetPieceAt(BoardState* state, int r, int c);
