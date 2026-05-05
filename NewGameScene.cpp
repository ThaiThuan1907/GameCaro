#include "NewGameScene.h"
#include "TextureManager.h"

void RenderNewGameScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnPvAI, UIButton btnPvP, SDL_Texture* texBack, bool hBack) {
    // Vẽ ảnh nền (Bạn có thể xài lại nền Menu hoặc một tấm nền sân Old Trafford góc khác cho mới mẻ)
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Vẽ 2 nút chọn chế độ
    DrawUIButton(btnPvAI, renderer);
    DrawUIButton(btnPvP, renderer);

    // VẼ NÚT BACK (GÓC TRÁI)
    SDL_Rect rectBack = { 18, 10, 89, 89 };
    if (hBack) {
        rectBack.x -= 5; rectBack.y -= 5; rectBack.w += 10; rectBack.h += 10;
        SDL_SetTextureColorMod(texBack, 255, 200, 200);
    }
    else {
        SDL_SetTextureColorMod(texBack, 255, 255, 255);
    }
    SDL_RenderCopy(renderer, texBack, NULL, &rectBack);
}

int HandleNewGameClick(int mouseX, int mouseY, UIButton btnPvAI, UIButton btnPvP, int currentState) {
    // Quy tắc 1 & 2: Khai báo 1 dòng 1 biến, khởi tạo an toàn
    int newState = currentState;

    if (mouseX >= 18 && mouseX <= 18 + 89 && mouseY >= 10 && mouseY <= 10 + 89) {
        return STATE_MENU; // Quay lại Menu chính
    }

    if (CheckMouseHover(btnPvAI, mouseX, mouseY) == true) {
        newState = STATE_PVAI_DIFFICULTY; // Rẽ nhánh sang màn hình chọn Easy, Normal, Hard
    }
    else if (CheckMouseHover(btnPvP, mouseX, mouseY) == true) {
        newState = STATE_PVP_INPUT_NAME;  // Rẽ nhánh sang màn hình nhập tên P1, P2
    }

    return newState;
}