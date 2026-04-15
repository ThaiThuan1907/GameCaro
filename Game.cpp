#include "Game.h"
#include <SDL_image.h>
#include "TextureManager.h"
#include "MenuScene.h"
#include "PlayScene.h"
#include "NewGameScene.h"
#include "PvAIScene.h"
#include "PvPScene.h"
#include "UITextBox.h"

Game::Game() {}
Game::~Game() {}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags | SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }

    if (TTF_Init() < 0) {
        std::cout << "Error: TTF_Init failed\n";
    }
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        // Logistics for mouse and other events can be moved here from main.cpp
        // For brevity in this step, I'm setting up the structure.
    }
}

void Game::Update() {
    // State updates logic
}

void Game::Render() {
    SDL_RenderClear(renderer);
    // Draw based on currentState
    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
}
