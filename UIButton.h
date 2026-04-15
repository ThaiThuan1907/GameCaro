#pragma once

#include <SDL.h>

// Khuôn m?u d? li?u cho m?t nút b?m
struct UIButton {
    // Quy t?c 1 & 2: 1 dòng 1 bi?n, b?t bu?c kh?i t?o
    int coordinateX = 0;
    int coordinateY = 0;
    int boxWidth = 0;
    int boxHeight = 0;
    SDL_Texture* buttonImage = NULL;
    bool isHovered = false; // Tr?ng thái ki?m tra xem chu?t có ?ang tr? vào không
};

// Kh?i t?o d? li?u cho nút
UIButton CreateButton(int startX, int startY, int width, int height, SDL_Texture* image);

// V? nút ra màn hình (Có kèm logic phóng to n?u tr? chu?t)
void DrawUIButton(UIButton button, SDL_Renderer* renderer);

// Ki?m tra xem chu?t có ?ang n?m trong không gian c?a nút không
bool CheckMouseHover(UIButton button, int mouseX, int mouseY);

