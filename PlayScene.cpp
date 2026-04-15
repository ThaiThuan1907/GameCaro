#include "PlayScene.h"
#include "TextureManager.h"
#include "CaroBoard.h"
#include "CaroAI.h"
#include "FileManager.h"
#include "UIButton.h"
#include <SDL.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

// Logic Phần 5
static CaroBoard boardLogic;
static CaroAI aiLogic;
static bool isGameFinished = false;
static int gameMode = 0; // 0: PvP, 1: PvAI
static int aiDifficulty = 0; 
static int staticCursorRow = BOARD_ROWS / 2;
static int staticCursorCol = BOARD_COLS / 2;

// Nút bấm UI
static UIButton btnRestart;
static UIButton btnHome;
static bool buttonsInitialized = false;

void InitGameOverButtons(SDL_Renderer* renderer) {
    if (buttonsInitialized) return;
    TTF_Font* font = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 64);
    if (!font) return;

    SDL_Surface* sRestart = TTF_RenderText_Solid(font, "RESTART", { 255, 255, 255, 255 });
    btnRestart = CreateButton(700, 600, sRestart->w, sRestart->h, SDL_CreateTextureFromSurface(renderer, sRestart));
    SDL_FreeSurface(sRestart);

    SDL_Surface* sHome = TTF_RenderText_Solid(font, "MENU", { 255, 255, 255, 255 });
    btnHome = CreateButton(1100, 600, sHome->w, sHome->h, SDL_CreateTextureFromSurface(renderer, sHome));
    SDL_FreeSurface(sHome);

    buttonsInitialized = true;
}

void SetGameMode(int mode, int difficulty) {
    gameMode = mode;
    aiDifficulty = difficulty;
    isGameFinished = false;
    boardLogic.Reset();
}

void ResetBoard(int board[BOARD_ROWS][BOARD_COLS]) {
    boardLogic.Reset();
    isGameFinished = false;
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            board[r][c] = 0;
        }
    }
}

void ProcessMove(int r, int c, int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, Uint32* turnStartTime) {
    if (isGameFinished) return;
    if (board[r][c] == 0) {
        board[r][c] = *currentPlayer;
        boardLogic.PlacePiece(r, c, *currentPlayer);
        if (boardLogic.CheckWin(r, c)) {
            isGameFinished = true;
            return;
        }
        *currentPlayer = (*currentPlayer == 1) ? 2 : 1;
        *turnStartTime = SDL_GetTicks();
    }
}

void CheckAIMove(int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, Uint32* turnStartTime) {
    if (!isGameFinished && gameMode == 1 && *currentPlayer == 2) {
        SDL_Delay(500); 
        int r, c;
        aiLogic.CalculateMove(boardLogic, aiDifficulty, r, c);
        ProcessMove(r, c, board, currentPlayer, turnStartTime);
    }
}

void DrawTextCenter(SDL_Renderer* renderer, TTF_Font* font, std::string text, int centerX, int centerY, SDL_Color color) {
    if (text == "" || font == NULL) return;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = { centerX - (surface->w / 2), centerY - (surface->h / 2), surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void RenderPlayScene(SDL_Renderer* renderer, SDL_Texture* bgPlaying, SDL_Texture* texBoard, SDL_Texture* texScoreboard, SDL_Texture* texInfoX, SDL_Texture* texInfoO, SDL_Texture* texFillInfo, SDL_Texture* texX, SDL_Texture* texO, int board[BOARD_ROWS][BOARD_COLS], TTF_Font* font64, TTF_Font* font36, std::string nameP1, std::string nameP2, int currentPlayer, int matchTimeInSeconds, int countdownTime) {
    InitGameOverButtons(renderer);
    
    // 1. Vẽ Nền và Các Bảng Thông Tin
    DrawTexture(bgPlaying, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DrawTexture(texInfoX, renderer, 42, 136, 460, 883);
    DrawTexture(texInfoO, renderer, 1418, 136, 460, 883);
    DrawTexture(texScoreboard, renderer, 441, 16, 1042, 194);
    DrawTexture(texBoard, renderer, BOARD_START_X, BOARD_START_Y, 840, 840);
    DrawTexture(texFillInfo, renderer, 73, 442, 395, 310);
    DrawTexture(texFillInfo, renderer, 1447, 442, 395, 310);

    // 2. Vẽ quân cờ & Con trỏ ảo
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            int drawX = BOARD_START_X + (c * CELL_SIZE);
            int drawY = BOARD_START_Y + (r * CELL_SIZE);
            if (!isGameFinished && r == staticCursorRow && c == staticCursorCol) {
                SDL_Rect cursorRect = { drawX, drawY, CELL_SIZE, CELL_SIZE };
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100); 
                SDL_RenderFillRect(renderer, &cursorRect);
            }
            if (board[r][c] != 0) {
                if (board[r][c] == 1) DrawTexture(texX, renderer, drawX + 4, drawY + 4, 62, 62);
                else DrawTexture(texO, renderer, drawX + 4, drawY + 4, 62, 62);
            }
        }
    }

    // 3. Vẽ Chữ và Đồng Hồ (HUD)
    SDL_Color colorRed = { 255, 0, 0, 255 };
    SDL_Color colorBlue = { 0, 0, 255, 255 };
    SDL_Color colorWhite = { 255, 255, 255, 255 };
    SDL_Color colorGreen = { 0, 255, 0, 255 };

    int dotCount = (SDL_GetTicks() / 500) % 4;
    std::string dots = "";
    for (int i = 0; i < dotCount; i++) dots += ".";

    // Thời gian trận đấu ở giữa trên cùng
    int minutes = matchTimeInSeconds / 60;
    int seconds = matchTimeInSeconds % 60;
    std::stringstream timeStr;
    timeStr << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    DrawTextCenter(renderer, font64, timeStr.str(), 955, 37, colorWhite);

    // Tên và thông tin người chơi 2 bên
    std::string finalP2Name = (gameMode == 1) ? "CPU Master" : (nameP2 == "" ? "Player 2" : nameP2);
    DrawTextCenter(renderer, font64, nameP1 == "" ? "Player 1" : nameP1, 751, 117, colorWhite);
    DrawTextCenter(renderer, font64, finalP2Name, 1172, 117, colorWhite);
    DrawTextCenter(renderer, font64, nameP1 == "" ? "Player 1" : nameP1, 271, 473, colorRed);
    DrawTextCenter(renderer, font64, finalP2Name, 1644, 473, colorBlue);

    // Đồng hồ đếm ngược 15s
    SDL_Color timeColor = (countdownTime <= 5) ? colorRed : colorWhite;
    std::string timeSecStr = std::to_string(countdownTime) + "s";

    if (currentPlayer == 1) {
        DrawTextCenter(renderer, font36, "YOUR TURN" + dots, 272, 558, colorGreen);
        DrawTextCenter(renderer, font64, "TIME", 270, 620, colorWhite);
        DrawTextCenter(renderer, font64, timeSecStr, 270, 680, timeColor);
        DrawTextCenter(renderer, font36, "AWAITING...", 1647, 558, colorWhite);
        DrawTextCenter(renderer, font64, "TIME", 1644, 620, colorWhite);
        DrawTextCenter(renderer, font64, "--", 1644, 680, colorWhite);
    } else {
        DrawTextCenter(renderer, font36, "AWAITING...", 272, 558, colorWhite);
        DrawTextCenter(renderer, font64, "TIME", 270, 620, colorWhite);
        DrawTextCenter(renderer, font64, "--", 270, 680, colorWhite);
        DrawTextCenter(renderer, font36, (gameMode == 1 ? "CPU THINKING" : "YOUR TURN") + dots, 1647, 558, colorGreen);
        DrawTextCenter(renderer, font64, "TIME", 1644, 620, colorWhite);
        DrawTextCenter(renderer, font64, timeSecStr, 1644, 680, timeColor);
    }

    if (isGameFinished) {
        SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &overlay);
        
        // --- PHẦN MỚI: HIỆN NGƯỜI THẮNG ---
        std::string winText = (currentPlayer == 1) ? "PLAYER 1 WINS!" : (gameMode == 1 ? "CPU WINS!" : "PLAYER 2 WINS!");
        DrawTextCenter(renderer, font64, winText, 960, 400, colorGreen);
        DrawTextCenter(renderer, font64, "G A M E   O V E R", 960, 480, colorRed);
        
        DrawUIButton(btnRestart, renderer);
        DrawUIButton(btnHome, renderer);
    }
}

int HandlePlayClick(int mouseX, int mouseY, int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, int currentState, Uint32* turnStartTime) {
    if (isGameFinished) {
        if (CheckMouseHover(btnRestart, mouseX, mouseY)) {
            ResetBoard(board);
            *turnStartTime = SDL_GetTicks();
            return currentState;
        }
        if (CheckMouseHover(btnHome, mouseX, mouseY)) {
            ResetBoard(board);
            return STATE_MENU;
        }
        return currentState;
    }
    if (gameMode == 1 && *currentPlayer == 2) return currentState;
    if (mouseX >= BOARD_START_X && mouseX <= BOARD_START_X + (BOARD_COLS * CELL_SIZE) &&
        mouseY >= BOARD_START_Y && mouseY <= BOARD_START_Y + (BOARD_ROWS * CELL_SIZE)) {
        int clickedCol = (mouseX - BOARD_START_X) / CELL_SIZE;
        int clickedRow = (mouseY - BOARD_START_Y) / CELL_SIZE;
        ProcessMove(clickedRow, clickedCol, board, currentPlayer, turnStartTime);
    }
    return currentState;
}

int HandlePlayKeyboard(SDL_Event& event, int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, int currentState, Uint32* turnStartTime, int& cursorRow, int& cursorCol) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            const SDL_MessageBoxButtonData buttons[] = {
                { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Save & Exit" },
                { 0,                                       1, "Exit" },
                { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Cancel" },
            };
            const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION, NULL, "Confirm Exit", "Save before exiting?", SDL_arraysize(buttons), buttons, NULL };
            int buttonid;
            if (SDL_ShowMessageBox(&messageboxdata, &buttonid) == 0) {
                if (buttonid == 0) { FileManager::SaveGame("quicksave.sav", board, *currentPlayer, 0, gameMode, aiDifficulty); return STATE_MENU; }
                else if (buttonid == 1) return STATE_MENU;
            }
            return currentState;
        }
        if (event.key.keysym.sym == SDLK_r) {
            ResetBoard(board);
            *turnStartTime = SDL_GetTicks();
            return currentState;
        }
        if (event.key.keysym.sym == SDLK_s && !isGameFinished) {
            if (FileManager::SaveGame("quicksave.sav", board, *currentPlayer, 0, gameMode, aiDifficulty)) {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Save Game", "Success!", NULL);
            }
        }
        if (isGameFinished || (gameMode == 1 && *currentPlayer == 2)) return currentState;
        switch (event.key.keysym.sym) {
            case SDLK_UP:    staticCursorRow = std::max(0, staticCursorRow - 1); break;
            case SDLK_DOWN:  staticCursorRow = std::min(BOARD_ROWS - 1, staticCursorRow + 1); break;
            case SDLK_LEFT:  staticCursorCol = std::max(0, staticCursorCol - 1); break;
            case SDLK_RIGHT: staticCursorCol = std::min(BOARD_COLS - 1, staticCursorCol + 1); break;
            case SDLK_SPACE: case SDLK_RETURN: ProcessMove(staticCursorRow, staticCursorCol, board, currentPlayer, turnStartTime); break;
        }
    }
    cursorRow = staticCursorRow;
    cursorCol = staticCursorCol;
    return currentState;
}