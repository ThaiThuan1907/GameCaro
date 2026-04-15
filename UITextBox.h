#pragma once

#include <SDL.h>
#include <SDL_ttf.h> 
#include <string>

// Khuôn mẫu dữ liệu cho Ô nhập chữ
struct UITextBox {
    int coordinateX = 0;
    int coordinateY = 0;
    int boxWidth = 0;
    int boxHeight = 0;
    std::string textContent = ""; // Nội dung chữ người chơi đã gõ
    bool isActive = false;        // Ô này có đang được click chọn để gõ không?
};

// Khởi tạo Ô nhập chữ
UITextBox CreateTextBox(int x, int y, int w, int h);

// Hàm xử lý tín hiệu bàn phím (Dùng con trỏ *textBox để có thể thay đổi nội dung bên trong)
void HandleTextBoxEvent(UITextBox* textBox, SDL_Event event);

// Hàm vẽ Ô nhập chữ và Nội dung chữ ra màn hình
void DrawTextBox(UITextBox textBox, SDL_Renderer* renderer, TTF_Font* font);