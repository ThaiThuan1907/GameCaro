#include "PvPScene.h"
#include "TextureManager.h"

void RenderPvPScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton lblTitle, UITextBox boxP1, UITextBox boxP2, UIButton btnStart, TTF_Font* font, UIButton lblInstruction, UIButton lblP1, UIButton lblP2, SDL_Texture* texBack, bool hBack) {
    // 1. Vẽ nền
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // 2. Vẽ TOÀN BỘ các nhãn dán chữ (Tiêu đề, Hướng dẫn, P1, P2)
    DrawUIButton(lblTitle, renderer);
    DrawUIButton(lblInstruction, renderer);
    DrawUIButton(lblP1, renderer);
    DrawUIButton(lblP2, renderer);

    // 3. Vẽ 2 ô nhập chữ (Khung chữ)
    DrawTextBox(boxP1, renderer, font);
    DrawTextBox(boxP2, renderer, font);

    // 4. Vẽ nút Bắt đầu
    DrawUIButton(btnStart, renderer);
}

// Hàm phụ trợ: Kiểm tra xem chuột có click trúng khung nhập chữ không
bool CheckTextBoxClick(UITextBox box, int mouseX, int mouseY) {
    if (mouseX >= box.coordinateX && mouseX <= box.coordinateX + box.boxWidth &&
        mouseY >= box.coordinateY && mouseY <= box.coordinateY + box.boxHeight) {
        return true;
    }
    return false;
}

int HandlePvPMouseClick(int mouseX, int mouseY, UITextBox* boxP1, UITextBox* boxP2, UIButton btnStart, int currentState) {
    int newState = currentState;

    if (mouseX >= 18 && mouseX <= 18 + 89 && mouseY >= 10 && mouseY <= 10 + 89) {
        return STATE_NEW_GAME_SELECT; // Lùi lại 1 bước ra màn hình Chọn PvP/PvAI
    }

    // 1. Kiểm tra click vào Ô nhập tên P1
    if (CheckTextBoxClick(*boxP1, mouseX, mouseY) == true) {
        boxP1->isActive = true;  // Bật ô 1
        boxP2->isActive = false; // Tắt ô 2
    }
    // 2. Kiểm tra click vào Ô nhập tên P2
    else if (CheckTextBoxClick(*boxP2, mouseX, mouseY) == true) {
        boxP2->isActive = true;  // Bật ô 2
        boxP1->isActive = false; // Tắt ô 1
    }
    // 3. Nếu click ra ngoài màn hình (không trúng ô nào) thì tắt cả 2 ô
    else {
        boxP1->isActive = false;
        boxP2->isActive = false;
    }

    // 4. Kiểm tra click vào nút Start Game
    if (CheckMouseHover(btnStart, mouseX, mouseY) == true) {
        // Khi bấm Start, chuyển sang màn hình chơi game luôn
        newState = STATE_PLAYING;
    }

    return newState;
}