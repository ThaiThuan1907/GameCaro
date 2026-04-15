#include "PvAIScene.h"
#include "TextureManager.h"

void RenderPvAIScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnEasy, UIButton btnMedium, UIButton btnHard) {
    // Tận dụng lại ảnh nền của phòng thay đồ (bgNewGame)
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Vẽ 3 nút độ khó
    DrawUIButton(btnEasy, renderer);
    DrawUIButton(btnMedium, renderer);
    DrawUIButton(btnHard, renderer);
}

#include "PlayScene.h"

int HandlePvAIClick(int mouseX, int mouseY, UIButton btnEasy, UIButton btnMedium, UIButton btnHard, int currentState) {
    int newState = currentState;

    if (CheckMouseHover(btnEasy, mouseX, mouseY) == true) {
        SetGameMode(1, 0); // PvAI, Easy
        newState = STATE_PLAYING;
    }
    else if (CheckMouseHover(btnMedium, mouseX, mouseY) == true) {
        SetGameMode(1, 1); // PvAI, Medium
        newState = STATE_PLAYING;
    }
    else if (CheckMouseHover(btnHard, mouseX, mouseY) == true) {
        SetGameMode(1, 2); // PvAI, Hard
        newState = STATE_PLAYING;
    }
    return newState;
}