#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Constants.h"

// Quy tắc 5: Tên Hàm viết hoa chữ cái đầu (PascalCase) [cite: 108-111]
class Game {
public:
    Game();
    ~Game();

    void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void HandleEvents();
    void Update();
    void Render();
    void Clean();

    bool Running() { return isRunning; }

private:
    // Quy tắc 2: Khởi tạo giá trị ban đầu [cite: 93-96]
    bool isRunning = false;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int currentState = STATE_MENU;
    int currentMouseX = 0;
    int currentMouseY = 0;
};
