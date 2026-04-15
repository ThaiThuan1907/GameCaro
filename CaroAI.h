#pragma once
#include "CaroBoard.h"

// Nhóm 5: Não Bộ & Luật Chơi - AI [cite: 153-156]
class CaroAI {
public:
    CaroAI();
    ~CaroAI();

    // Tính toán nước đi dựa trên độ khó
    void CalculateMove(CaroBoard& board, int difficulty, int& outRow, int& outCol);

private:
    void RandomMove(CaroBoard& board, int& outRow, int& outCol);
    void SmartMove(CaroBoard& board, int& outRow, int& outCol); // Pseudocode or basic scoring
};
