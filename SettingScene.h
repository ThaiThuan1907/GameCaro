#pragma once

#include <SDL.h>
#include "Constants.h"

// Hàm vẽ màn hình Setting
void RenderSettingScene(SDL_Renderer* renderer, SDL_Texture* bgSetting, SDL_Texture* texPanel, SDL_Texture* texVanDijk,
    SDL_Texture* btnSound, SDL_Texture* btnControl, SDL_Texture* btnRules, SDL_Texture* btnAbout,
    SDL_Texture* texLoa, SDL_Texture* texSFXIcon, SDL_Texture* texTrack1, SDL_Texture* texBG1, SDL_Texture* texFill1, SDL_Texture* texKnob1,
    SDL_Texture* texTrack2, SDL_Texture* texFill2, SDL_Texture* texKnob2,
    SDL_Texture* texPanelControl, SDL_Texture* texBall1, SDL_Texture* texBall2, // <--- THÊM 2 ẢNH QUẢ BÓNG
    SDL_Texture* texClose,
    int currentTab, int musicVolume, int sfxVolume, int controlMode, // <--- THÊM BIẾN controlMode
    SDL_Texture* texRules, SDL_Texture* texAbout,
    bool hSound, bool hControl, bool hRules, bool hAbout,   
    bool hBall1, bool hBall2,
    bool hClose);

// Hàm bắt sự kiện Click và Drag chuột
void HandleSettingEvents(SDL_Event& event, int* currentTab, int* musicVolume, int* sfxVolume, int* controlMode, // <--- THÊM BIẾN controlMode
    bool* isDraggingMusic, bool* isDraggingSFX, int& currentState);