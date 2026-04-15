#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

// Hàm tải ảnh từ ổ cứng lên RAM
SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer);

// Hàm vẽ hình ảnh ra màn hình
void DrawTexture(SDL_Texture* texture, SDL_Renderer* renderer, int coordinateX, int coordinateY, int boxWidth, int boxHeight);