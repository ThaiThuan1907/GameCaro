#include "MenuScene.h"
#include "TextureManager.h"

void RenderMenuScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit) {
    // 1. V? ?nh n?n ph? kín màn hình
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // 2. V? 4 nút lên trên ?nh n?n
    DrawUIButton(btnNewGame, renderer);
    DrawUIButton(btnLoadGame, renderer);
    DrawUIButton(btnSetting, renderer);
    DrawUIButton(btnExit, renderer);
}

int HandleMenuClick(int mouseX, int mouseY, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit, int currentState) {
    // Quy t?c 1 & 2: Kh?i t?o bi?n l?u tr?ng thái m?i
    int newState = currentState;

    // Dùng hàm CheckMouseHover (b?n ?ã có ? file UIButton) ?? xem chu?t có trúng nút không
    if (CheckMouseHover(btnNewGame, mouseX, mouseY) == true) {
        newState = STATE_NEW_GAME_SELECT; // Chuy?n sang màn ch?n PVAI/PVP
    }
    else if (CheckMouseHover(btnLoadGame, mouseX, mouseY) == true) {
        newState = STATE_LOAD_GAME;       // Chuy?n sang màn Load Game
    }
    else if (CheckMouseHover(btnSetting, mouseX, mouseY) == true) {
        newState = STATE_SETTINGS_SOUND;  // Chuy?n sang tab Sound c?a Setting
    }
    else if (CheckMouseHover(btnExit, mouseX, mouseY) == true) {
        newState = -1; // Mã ??c bi?t: -1 ngh?a là thoát game
    }

    return newState;
}