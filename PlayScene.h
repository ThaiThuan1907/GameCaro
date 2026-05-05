#pragma once

#include <SDL.h>
#include <SDL_ttf.h> 
#include "Constants.h"

#include <string>    
#include <iomanip>  
#include <sstream>

// Hàm dọn dẹp bàn cờ
void ResetBoard(int board[BOARD_ROWS][BOARD_COLS]);

// Hàm phụ trợ in chữ căn giữa
void DrawTextCenter(SDL_Renderer* renderer, TTF_Font* font, std::string text, int centerX, int centerY, SDL_Color color);

// HÀM VẼ GIAO DIỆN CHÍNH (Khai báo phải chuẩn xác 100% tham số)
void RenderPlayScene(SDL_Renderer* renderer, SDL_Texture* bgPlaying, SDL_Texture* texBoard, SDL_Texture* texScoreboard, SDL_Texture* texInfoX, 
	SDL_Texture* texInfoO, SDL_Texture* texFillInfo, SDL_Texture* texX, SDL_Texture* texO, int board[BOARD_ROWS][BOARD_COLS], TTF_Font* font64, 
	TTF_Font* font36, std::string nameP1, std::string nameP2, int currentPlayer, int matchTimeInSeconds, int countdownTime,
	int controlMode, int cursorRow, int cursorCol);

// HÀM XỬ LÝ CLICK
int HandlePlayClick(int mouseX, int mouseY, int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, int currentState, Uint32* turnStartTime);