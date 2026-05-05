#include "PvAIScene.h"
#include "TextureManager.h"

void RenderPvAIScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnEasy, UIButton btnMedium, UIButton btnHard, SDL_Texture* texBack, bool hBack) {
    // Tận dụng lại ảnh nền của phòng thay đồ (bgNewGame)
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Vẽ 3 nút độ khó
    DrawUIButton(btnEasy, renderer);
    DrawUIButton(btnMedium, renderer);
    DrawUIButton(btnHard, renderer);
}

int HandlePvAIClick(int mouseX, int mouseY, UIButton btnEasy, UIButton btnMedium, UIButton btnHard, int currentState) {
    int newState = currentState;

    if (mouseX >= 18 && mouseX <= 18 + 89 && mouseY >= 10 && mouseY <= 10 + 89) {
        return STATE_NEW_GAME_SELECT; // Lùi lại 1 bước ra màn hình Chọn PvP/PvAI
    }

    // Tạm thời, dù chọn độ khó nào thì cũng nhảy thẳng vào màn hình chơi game (STATE_PLAYING)
    if (CheckMouseHover(btnEasy, mouseX, mouseY) == true) {
        newState = STATE_PLAYING;
    }
    else if (CheckMouseHover(btnMedium, mouseX, mouseY) == true) {
        newState = STATE_PLAYING;
    }
    else if (CheckMouseHover(btnHard, mouseX, mouseY) == true) {
        newState = STATE_PLAYING;
    }

    return newState;
}