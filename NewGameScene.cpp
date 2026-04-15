#include "NewGameScene.h"
#include "TextureManager.h"

void RenderNewGameScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnPvAI, UIButton btnPvP) {
    // Vẽ ảnh nền (Bạn có thể xài lại nền Menu hoặc một tấm nền sân Old Trafford góc khác cho mới mẻ)
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Vẽ 2 nút chọn chế độ
    DrawUIButton(btnPvAI, renderer);
    DrawUIButton(btnPvP, renderer);
}

int HandleNewGameClick(int mouseX, int mouseY, UIButton btnPvAI, UIButton btnPvP, int currentState) {
    // Quy tắc 1 & 2: Khai báo 1 dòng 1 biến, khởi tạo an toàn
    int newState = currentState;

    if (CheckMouseHover(btnPvAI, mouseX, mouseY) == true) {
        newState = STATE_PVAI_DIFFICULTY; // Rẽ nhánh sang màn hình chọn Easy, Normal, Hard
    }
    else if (CheckMouseHover(btnPvP, mouseX, mouseY) == true) {
        newState = STATE_PVP_INPUT_NAME;  // Rẽ nhánh sang màn hình nhập tên P1, P2
    }

    return newState;
}