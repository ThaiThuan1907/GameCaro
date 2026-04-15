#pragma once

#include <SDL.h>
#include "UIButton.h"
#include "Constants.h"

// Hàm vẽ giao diện chọn độ khó
void RenderPvAIScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnEasy, UIButton btnMedium, UIButton btnHard);

// Hàm xử lý click chuột chọn độ khó
int HandlePvAIClick(int mouseX, int mouseY, UIButton btnEasy, UIButton btnMedium, UIButton btnHard, int currentState);