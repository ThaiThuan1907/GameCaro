#include "CaroBoard.h"

CaroBoard::CaroBoard() {
    Init();
}

CaroBoard::~CaroBoard() {}

void CaroBoard::Init() {
    Reset();
}

void CaroBoard::Reset() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            board[i][j] = 0;
        }
    }
    cursorRow = BOARD_ROWS / 2;
    cursorCol = BOARD_COLS / 2;
}

bool CaroBoard::PlacePiece(int row, int col, int player) {
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) return false;
    if (board[row][col] != 0) return false;
    
    board[row][col] = player;
    return true;
}

void CaroBoard::MoveCursor(int dr, int dc) {
    cursorRow += dr;
    cursorCol += dc;

    if (cursorRow < 0) cursorRow = 0;
    if (cursorRow >= BOARD_ROWS) cursorRow = BOARD_ROWS - 1;
    if (cursorCol < 0) cursorCol = 0;
    if (cursorCol >= BOARD_COLS) cursorCol = BOARD_COLS - 1;
}

void CaroBoard::GetCursorPos(int& r, int& c) {
    r = cursorRow;
    c = cursorCol;
}

bool CaroBoard::CheckWin(int row, int col) {
    int player = board[row][col];
    if (player == 0) return false;

    // 4 hướng: Ngang, Dọc, Chéo xuôi ( \ ), Chéo ngược ( / )
    int dr[] = { 0, 1, 1, 1 };
    int dc[] = { 1, 0, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int count = 1;
        
        // Đi về phía trước
        int r = row + dr[i];
        int c = col + dc[i];
        while (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS && board[r][c] == player) {
            count++;
            r += dr[i];
            c += dc[i];
        }

        // Đi về phía ngược lại
        r = row - dr[i];
        c = col - dc[i];
        while (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS && board[r][c] == player) {
            count++;
            r -= dr[i];
            c -= dc[i];
        }

        // Đủ 5 quân liên tiếp là thắng [cite: 151]
        if (count >= 5) return true;
    }

    return false;
}
