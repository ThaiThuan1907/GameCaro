#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "Constants.h"
#include "UIButton.h"
#include "UITextBox.h"

// Hàm vẽ màn hình Load Game
void RenderLoadGameScene(SDL_Renderer* renderer, SDL_Texture* bgMenu, SDL_Texture* texSearchBar, SDL_Texture* texPanel, UIButton btnLoad, UIButton btnRename, UIButton btnDelete, UITextBox* searchBox, std::vector<std::string> fileList, int selectedFileIndex, int scrollOffset, TTF_Font* font60, SDL_Texture* texClose, bool hClose);

// Hàm bắt click chuột
int HandleLoadGameClick(int mouseX, int mouseY, UIButton btnLoad, UIButton btnRename, UIButton btnDelete, UITextBox* searchBox, std::vector<std::string> fileList, int* selectedFileIndex, int scrollOffset, int currentState);