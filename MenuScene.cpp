#include "MenuScene.h"
#include "TextureManager.h"

void RenderMenuScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit) {
    DrawTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DrawUIButton(btnNewGame, renderer);
    DrawUIButton(btnLoadGame, renderer);
    DrawUIButton(btnSetting, renderer);
    DrawUIButton(btnExit, renderer);
}

int HandleMenuClick(int mouseX, int mouseY, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit, int currentState) {
    int newState = currentState;
    if (CheckMouseHover(btnNewGame, mouseX, mouseY) == true) {
        newState = STATE_NEW_GAME_SELECT;
    }
    else if (CheckMouseHover(btnLoadGame, mouseX, mouseY) == true) {
        newState = STATE_LOAD_GAME;
    }
    else if (CheckMouseHover(btnSetting, mouseX, mouseY) == true) {
        newState = STATE_SETTING;
    }
    else if (CheckMouseHover(btnExit, mouseX, mouseY) == true) {
        newState = -1; 
    }
    return newState;
}
