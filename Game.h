#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Constants.h"

using namespace std;

// --- Biến toàn cục của Game (thay thế cho member variables trong class) ---
extern bool   isRunning;
extern SDL_Window*   gameWindow;
extern SDL_Renderer* gameRenderer;
extern int    currentState;
extern int    currentMouseX;
extern int    currentMouseY;

// --- Các hàm thủ tục thay thế cho method của class Game ---
void GameInit(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
void GameHandleEvents();
void GameUpdate();
void GameRender();
void GameClean();
bool GameIsRunning();
