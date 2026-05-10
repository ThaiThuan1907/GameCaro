#include "CaroBoard.h"

void InitBoardState(BoardState* state) {
    ResetBoardState(state);
}

void ResetBoardState(BoardState* state) {
    if (!state) return;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            state->board[i][j] = 0;
        }
    }
    state->cursorRow = BOARD_ROWS / 2;
    state->cursorCol = BOARD_COLS / 2;
    for (int i = 0; i < 5; i++) {
        state->winningCoords[i] = { -1, -1 };
    }
}

bool PlacePiece(BoardState* state, int row, int col, int player) {
    if (!state) return false;
    if (row < 0 || row >= BOARD_ROWS || col < 0 || col >= BOARD_COLS) return false;
    if (state->board[row][col] != 0) return false;
    
    state->board[row][col] = player;
    return true;
}

void MoveBoardCursor(BoardState* state, int dr, int dc) {
    if (!state) return;
    state->cursorRow += dr;
    state->cursorCol += dc;

    if (state->cursorRow < 0) state->cursorRow = 0;
    if (state->cursorRow >= BOARD_ROWS) state->cursorRow = BOARD_ROWS - 1;
    if (state->cursorCol < 0) state->cursorCol = 0;
    if (state->cursorCol >= BOARD_COLS) state->cursorCol = BOARD_COLS - 1;
}

int GetPieceAt(BoardState* state, int r, int c) {
    if (!state || r < 0 || r >= BOARD_ROWS || c < 0 || c >= BOARD_COLS) return 0;
    return state->board[r][c];
}

bool CheckWin(BoardState* state, int row, int col) {
    if (!state) return false;
    int player = state->board[row][col];
    if (player == 0) return false;

    // 4 hướng: Ngang, Dọc, Chéo xuôi ( \ ), Chéo ngược ( / )
    int dr[] = { 0, 1, 1, 1 };
    int dc[] = { 1, 0, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int count = 1;
        std::vector<BoardState::Coord> tempCoords;
        tempCoords.push_back({ row, col });

        // Đi về phía trước
        int r = row + dr[i];
        int c = col + dc[i];
        while (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS && state->board[r][c] == player) {
            tempCoords.push_back({ r, c });
            count++;
            r += dr[i];
            c += dc[i];
        }

        // Đi về phía ngược lại
        r = row - dr[i];
        c = col - dc[i];
        while (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS && state->board[r][c] == player) {
            tempCoords.push_back({ r, c });
            count++;
            r -= dr[i];
            c -= dc[i];
        }

        // Đủ 5 quân liên tiếp là thắng
        if (count >= 5) {
            // Lưu lại 5 quân đầu tiên tìm thấy trong chuỗi thắng (hoặc tất cả nếu muốn)
            for (int k = 0; k < 5; k++) {
                state->winningCoords[k] = tempCoords[k];
            }
            return true;
        }
    }

    return false;
}

bool IsBoardFull(BoardState* state) {
    if (!state) return false;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (state->board[i][j] == 0) return false;
        }
    }
    return true;
}
