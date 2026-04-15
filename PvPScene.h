#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "UIButton.h"
#include "UITextBox.h"
#include "Constants.h"

// Hàm vẽ giao diện PVP
void RenderPvPScene(SDL_Renderer* renderer, SDL_Texture* background, UIButton lblTitle, UITextBox boxP1, UITextBox boxP2, UIButton btnStart, TTF_Font* font, UIButton lblInstruction, UIButton lblP1, UIButton lblP2);

// Hàm xử lý Click chuột (Lưu ý: Truyền con trỏ &boxP1, &boxP2 để có thể thay đổi trạng thái nhấp nháy của nó)
int HandlePvPMouseClick(int mouseX, int mouseY, UITextBox* boxP1, UITextBox* boxP2, UIButton btnStart, int currentState);