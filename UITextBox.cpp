#include "UITextBox.h"

UITextBox CreateTextBox(int x, int y, int w, int h) {
    UITextBox newBox;
    newBox.coordinateX = x;
    newBox.coordinateY = y;
    newBox.boxWidth = w;
    newBox.boxHeight = h;
    newBox.textContent = "";
    newBox.isActive = false;
    return newBox;
}

void HandleTextBoxEvent(UITextBox* textBox, SDL_Event event) {
    // Nếu ô này không được chọn (isActive = false), thì bỏ qua không nhận phím
    if (textBox->isActive == false) {
        return;
    }

    // 1. Xử lý nút Xóa lùi (Backspace)
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && textBox->textContent.length() > 0) {
            textBox->textContent.pop_back(); // Xóa ký tự cuối cùng
        }
    }
    // 2. Xử lý gõ chữ cái và số (Text Input)
    else if (event.type == SDL_TEXTINPUT) {
        // Giới hạn tên người chơi tối đa 15 ký tự để không bị tràn khung
        if (textBox->textContent.length() < 35) {
            textBox->textContent += event.text.text; // Nối chữ vừa gõ vào chuỗi
        }
    }
}

void DrawTextBox(UITextBox textBox, SDL_Renderer* renderer, TTF_Font* font) {
    // 1. Vẽ cái khung nền (Nền trắng, viền đen)
    SDL_Rect boxRect = { textBox.coordinateX, textBox.coordinateY, textBox.boxWidth, textBox.boxHeight };

    // Nếu ô đang được click chọn, tô nền màu xám nhạt để nhận biết, ngược lại tô màu trắng
    if (textBox.isActive == true) {
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_RenderFillRect(renderer, &boxRect); // Tô màu nền

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Chọn màu đen cho viền
    SDL_RenderDrawRect(renderer, &boxRect);         // Vẽ viền

    // 2. Vẽ chữ lên trên (Nếu đã có chữ và font tải thành công)
    if (textBox.textContent.length() > 0 && font != NULL) {
        SDL_Color textColor = { 0, 0, 0, 255 }; // Chữ màu đen

        // Render chữ thành một tấm ảnh (Surface)
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBox.textContent.c_str(), textColor);
        if (textSurface != NULL) {
            // Chuyển Surface thành Texture để đưa lên Card màn hình
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            // Căn chỉnh chữ nằm cách viền trái và viền trên 10 pixel cho đẹp
            SDL_Rect textRect = { textBox.coordinateX + 10, textBox.coordinateY + 10, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            // Dọn dẹp RAM
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }
}