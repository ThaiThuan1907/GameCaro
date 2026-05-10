#pragma once
#include "CaroBoard.h"

// Nhóm 5: Não Bộ & Luật Chơi - AI
// Tính toán nước đi dựa trên độ khó
void CalculateAIMove(BoardState* board, int difficulty, int& outRow, int& outCol);

// Các hàm phụ trợ logic AI
void AIMakeRandomMove(BoardState* board, int& outRow, int& outCol);
void AIMakeSmartMove(BoardState* board, int& outRow, int& outCol);
