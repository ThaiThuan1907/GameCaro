#pragma once

#include <SDL.h>
#include "UIButton.h"
#include "Constants.h"

// Hàm vẽ giao diện chọn chế độ chơi
void RenderNewGameScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnPvAI, UIButton btnPvP, SDL_Texture* texBack, bool hBack);

// Hàm xử lý click chuột để rẽ nhánh sang Chọn Độ Khó hoặc Nhập Tên
int HandleNewGameClick(int mouseX, int mouseY, UIButton btnPvAI, UIButton btnPvP, int currentState);