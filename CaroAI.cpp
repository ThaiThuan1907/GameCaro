#include "CaroAI.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

CaroAI::CaroAI() {
    srand((unsigned int)time(NULL));
}

CaroAI::~CaroAI() {}

// Hàm chấm điểm cho một ô cờ dựa trên các hướng xung quanh
int EvaluateCell(CaroBoard& board, int r, int c, int player) {
    int score = 0;
    int opponent = (player == 1) ? 2 : 1;

    int dr[] = { 0, 1, 1, 1 };
    int dc[] = { 1, 0, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int countPlayer = 0;
        int countOpponent = 0;

        // Kiểm tra về 2 phía của hướng i
        for (int k = -4; k <= 4; k++) {
            if (k == 0) continue;
            int nr = r + dr[i] * k;
            int nc = c + dc[i] * k;

            if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) {
                if (board.GetPiece(nr, nc) == player) countPlayer++;
                else if (board.GetPiece(nr, nc) == opponent) countOpponent++;
            }
        }

        // Ưu tiên chặn đối thủ nếu họ sắp thắng (4 quân)
        if (countOpponent >= 4) score += 10000;
        else if (countOpponent >= 3) score += 1000;
        
        // Ưu tiên tự nối dài quân mình
        if (countPlayer >= 4) score += 50000; // Có thể thắng ngay
        else if (countPlayer >= 3) score += 5000;
        else if (countPlayer >= 2) score += 500;
        else if (countPlayer >= 1) score += 50;
    }
    return score;
}

void CaroAI::CalculateMove(CaroBoard& board, int difficulty, int& outRow, int& outCol) {
    if (difficulty == 0) { // EASY: Đánh bừa (Random) [cite: 155]
        RandomMove(board, outRow, outCol);
        return;
    }

    // MEDIUM & HARD: Dùng Heuristic chấm điểm [cite: 156]
    int bestScore = -1;
    std::vector<std::pair<int, int>> bestMoves;

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (board.GetPiece(i, j) == 0) {
                int currentScore = EvaluateCell(board, i, j, 2); // Giả định AI là Player 2
                
                // Hard thì tính toán kỹ hơn (cộng thêm điểm phòng thủ)
                if (difficulty == 2) {
                    currentScore += EvaluateCell(board, i, j, 1) * 1.5; // Chú trọng chặn rào chéo
                }

                if (currentScore > bestScore) {
                    bestScore = currentScore;
                    bestMoves.clear();
                    bestMoves.push_back({ i, j });
                } else if (currentScore == bestScore) {
                    bestMoves.push_back({ i, j });
                }
            }
        }
    }

    if (!bestMoves.empty()) {
        int idx = rand() % bestMoves.size();
        outRow = bestMoves[idx].first;
        outCol = bestMoves[idx].second;
    } else {
        RandomMove(board, outRow, outCol);
    }
}

void CaroAI::RandomMove(CaroBoard& board, int& outRow, int& outCol) {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (board.GetPiece(i, j) == 0) emptyCells.push_back({ i, j });
        }
    }
    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();
        outRow = emptyCells[idx].first;
        outCol = emptyCells[idx].second;
    }
}

void CaroAI::SmartMove(CaroBoard& board, int& outRow, int& outCol) {
    // Đã gộp logic vào CalculateMove để xử lý theo difficulty
}
