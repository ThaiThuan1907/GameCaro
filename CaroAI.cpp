#include "CaroAI.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <climits>

// --- THIẾT LẬP ĐIỂM SỐ CHO CÁC THẾ CỜ ---
const int SCORE_WIN = 100000000;
const int SCORE_LIVE_4 = 1000000;
const int SCORE_DEAD_4 = 100000;
const int SCORE_LIVE_3 = 10000;
const int SCORE_DEAD_3 = 1000;
const int SCORE_LIVE_2 = 100;
const int SCORE_DEAD_2 = 10;

// Hàm tính điểm cho một chuỗi quân cờ
int GetLineScore(int count, int blocks) {
    if (blocks == 2 && count < 5) return 0;
    switch (count) {
    case 5: return SCORE_WIN;
    case 4: return (blocks == 0) ? SCORE_LIVE_4 : SCORE_DEAD_4;
    case 3: return (blocks == 0) ? SCORE_LIVE_3 : SCORE_DEAD_3;
    case 2: return (blocks == 0) ? SCORE_LIVE_2 : SCORE_DEAD_2;
    default: return (count >= 5) ? SCORE_WIN : 0;
    }
}

// Hàm đánh giá nhanh cho một ô cờ (dùng để sắp xếp nước đi)
int QuickEvaluateCell(BoardState* board, int r, int c, int player) {
    int score = 0;
    int opponent = (player == 1) ? 2 : 1;
    int dr[] = { 0, 1, 1, 1 }, dc[] = { 1, 0, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int count = 0, blocks = 0;
        // Kiểm tra 2 hướng
        for (int dir = -1; dir <= 1; dir += 2) {
            for (int step = 1; step <= 4; step++) {
                int nr = r + dr[i] * step * dir, nc = c + dc[i] * step * dir;
                if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) {
                    int p = board->board[nr][nc];
                    if (p == player) count++;
                    else if (p == opponent) { blocks++; break; }
                    else break;
                } else { blocks++; break; }
            }
        }
        score += GetLineScore(count + 1, blocks);
    }
    return score;
}

// Hàm đánh giá toàn bộ bàn cờ
int EvaluateBoard(BoardState* board, int player) {
    int totalScore = 0;
    int opponent = (player == 1) ? 2 : 1;
    int dr[] = { 0, 1, 1, 1 }, dc[] = { 1, 0, 1, -1 };
    bool visited[4][BOARD_ROWS][BOARD_COLS] = { false };

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (board->board[r][c] == player) {
                for (int i = 0; i < 4; i++) {
                    if (visited[i][r][c]) continue;
                    int count = 1, blocks = 0;
                    int nr = r + dr[i], nc = c + dc[i];
                    while (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS && board->board[nr][nc] == player) {
                        visited[i][nr][nc] = true;
                        count++; nr += dr[i]; nc += dc[i];
                    }
                    if (!(nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) || board->board[nr][nc] == opponent) blocks++;
                    nr = r - dr[i]; nc = c - dc[i];
                    while (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS && board->board[nr][nc] == player) {
                        count++; nr -= dr[i]; nc -= dc[i];
                    }
                    if (!(nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS) || board->board[nr][nc] == opponent) blocks++;
                    totalScore += GetLineScore(count, blocks);
                }
            }
        }
    }
    return totalScore;
}

int EvaluatePosition(BoardState* board) {
    return EvaluateBoard(board, 2) - (int)(EvaluateBoard(board, 1) * 1.5);
}

struct Move { int r, c, score; };
bool CompareMoves(const Move& a, const Move& b) { return a.score > b.score; }

std::vector<Move> GetCandidateMoves(BoardState* board, int player) {
    std::vector<Move> moves;
    bool hasNeighbors[BOARD_ROWS][BOARD_COLS] = { false };
    bool boardEmpty = true;

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (board->board[r][c] != 0) {
                boardEmpty = false;
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < BOARD_ROWS && nc >= 0 && nc < BOARD_COLS && board->board[nr][nc] == 0)
                            hasNeighbors[nr][nc] = true;
                    }
                }
            }
        }
    }

    if (boardEmpty) { moves.push_back({ BOARD_ROWS / 2, BOARD_COLS / 2, 0 }); return moves; }

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (hasNeighbors[r][c]) {
                int score = QuickEvaluateCell(board, r, c, player) + QuickEvaluateCell(board, r, c, (player == 1 ? 2 : 1));
                moves.push_back({ r, c, score });
            }
        }
    }
    std::sort(moves.begin(), moves.end(), CompareMoves);
    // Chỉ lấy tối đa 15 nước đi tiềm năng nhất để giảm nhánh
    if (moves.size() > 15) moves.resize(15);
    return moves;
}

int Minimax(BoardState* board, int depth, int alpha, int beta, bool isMaximizing) {
    if (depth == 0) return EvaluatePosition(board);

    std::vector<Move> moves = GetCandidateMoves(board, isMaximizing ? 2 : 1);
    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (auto& move : moves) {
            board->board[move.r][move.c] = 2;
            int eval = Minimax(board, depth - 1, alpha, beta, false);
            board->board[move.r][move.c] = 0;
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (auto& move : moves) {
            board->board[move.r][move.c] = 1;
            int eval = Minimax(board, depth - 1, alpha, beta, true);
            board->board[move.r][move.c] = 0;
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

void CalculateAIMove(BoardState* board, int difficulty, int& outRow, int& outCol) {
    if (!board) return;
    int depth = (difficulty == 0) ? 1 : (difficulty == 1 ? 2 : 4);
    std::vector<Move> moves = GetCandidateMoves(board, 2);
    int bestScore = INT_MIN;
    std::vector<std::pair<int, int>> bestMoves;

    for (auto& move : moves) {
        board->board[move.r][move.c] = 2;
        int score = Minimax(board, depth - 1, INT_MIN, INT_MAX, false);
        board->board[move.r][move.c] = 0;
        if (score > bestScore) {
            bestScore = score; bestMoves.clear(); bestMoves.push_back({ move.r, move.c });
        } else if (score == bestScore) {
            bestMoves.push_back({ move.r, move.c });
        }
    }
    if (!bestMoves.empty()) {
        int idx = rand() % (int)bestMoves.size();
        outRow = bestMoves[idx].first; outCol = bestMoves[idx].second;
    } else {
        outRow = BOARD_ROWS / 2; outCol = BOARD_COLS / 2;
    }
}

void AIMakeRandomMove(BoardState* board, int& outRow, int& outCol) { CalculateAIMove(board, 0, outRow, outCol); }
void AIMakeSmartMove(BoardState* board, int& outRow, int& outCol) { CalculateAIMove(board, 2, outRow, outCol); }
