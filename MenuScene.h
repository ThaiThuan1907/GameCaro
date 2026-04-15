#pragma once

#include <SDL.h>
#include "UIButton.h"
#include "Constants.h"

// Hàm v? toàn b? giao di?n Menu ra màn hình
// Input: B? v? (renderer), ?nh n?n, và 4 nút ch?c n?ng
void RenderMenuScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit);

// Hàm x? lý khi ng??i dùng click chu?t trái trong Menu
// Input: T?a ?? chu?t (x, y), 4 nút ch?c n?ng, và Tr?ng thái hi?n t?i
// Output: Tr? v? m?t GameState M?I ?? báo cho h? th?ng bi?t ph?i chuy?n ?i ?âu
int HandleMenuClick(int mouseX, int mouseY, UIButton btnNewGame, UIButton btnLoadGame, UIButton btnSetting, UIButton btnExit, int currentState);