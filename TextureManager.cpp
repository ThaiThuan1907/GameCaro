#include "TextureManager.h"
#include <iostream>

SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer) 
{
    SDL_Texture* finalTexture = NULL;
    SDL_Surface* loadedSurface = NULL;

    // Load ?nh thành d?ng Surface (d? li?u thô trên RAM)
    loadedSurface = IMG_Load(filePath);
    if (loadedSurface == NULL) 
    {
        std::cout << "Loi load anh! SDL_image Error: " << IMG_GetError() << "\n";
        return NULL;
    }

    // Chuy?n t? Surface sang Texture (d? li?u t?i ?u cho Card màn hình)
    finalTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (finalTexture == NULL) 
    {
        std::cout << "Loi tao Texture! SDL Error: " << SDL_GetError() << "\n";
    }

    // D?n d?p Surface thô
    SDL_FreeSurface(loadedSurface);

    return finalTexture;
}

void DrawTexture(SDL_Texture* texture, SDL_Renderer* renderer, int coordinateX, int coordinateY, int boxWidth, int boxHeight) 
{
    // Quy t?c 3: Tên bi?n camelCase
    SDL_Rect destinationRect;
    destinationRect.x = coordinateX;
    destinationRect.y = coordinateY;
    destinationRect.w = boxWidth;
    destinationRect.h = boxHeight;

    // Copy ?nh vào b? v?
    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
}