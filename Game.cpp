#include "Game.h"
#include <SDL_image.h>
#include "TextureManager.h"
#include "MenuScene.h"
#include "PlayScene.h"
#include "NewGameScene.h"
#include "PvAIScene.h"
#include "PvPScene.h"
#include "UITextBox.h"

using namespace std;

// --- Định nghĩa các biến toàn cục (khai báo extern ở Game.h) ---
bool   isRunning     = false;
SDL_Window*   gameWindow   = NULL;
SDL_Renderer* gameRenderer = NULL;
int    currentState  = STATE_MENU;
int    currentMouseX = 0;
int    currentMouseY = 0;

// Khởi tạo SDL, cửa sổ và renderer
void GameInit(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        gameWindow   = SDL_CreateWindow(title, xpos, ypos, width, height, flags | SDL_WINDOW_RESIZABLE);
        gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);
        if (gameRenderer) {
            SDL_RenderSetLogicalSize(gameRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }

    if (TTF_Init() < 0) {
        cout << "Error: TTF_Init failed\n";
    }
}

// Xử lý sự kiện chuột/bàn phím
void GameHandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (event.type == SDL_MOUSEMOTION) {
            currentMouseX = event.motion.x;
            currentMouseY = event.motion.y;
        }
    }
}

// Cập nhật trạng thái game
void GameUpdate() {
    // Logic cập nhật state machine đặt ở đây
}

// Vẽ frame hiện tại
void GameRender() {
    SDL_RenderClear(gameRenderer);
    // Vẽ dựa vào currentState
    SDL_RenderPresent(gameRenderer);
}

// Dọn dẹp tài nguyên khi thoát
void GameClean() {
    SDL_DestroyWindow(gameWindow);
    SDL_DestroyRenderer(gameRenderer);
    TTF_Quit();
    SDL_Quit();
}

// Trả về trạng thái game còn chạy không
bool GameIsRunning() {
    return isRunning;
}
