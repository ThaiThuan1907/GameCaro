#include "PlayScene.h"
#include "TextureManager.h"

void ResetBoard(int board[BOARD_ROWS][BOARD_COLS]) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            board[r][c] = 0; // Đặt toàn bộ 144 ô (12x12) về trạng thái trống
        }
    }
}

void DrawTextCenter(SDL_Renderer* renderer, TTF_Font* font, std::string text, int centerX, int centerY, SDL_Color color) {
    if (text == "" || font == NULL) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // Trừ đi một nửa chiều rộng và chiều cao để đặt đúng tâm
        SDL_Rect rect = { centerX - (surface->w / 2), centerY - (surface->h / 2), surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void RenderPlayScene(SDL_Renderer* renderer, SDL_Texture* bgPlaying, SDL_Texture* texBoard, SDL_Texture* texScoreboard, SDL_Texture* texInfoX,
    SDL_Texture* texInfoO, SDL_Texture* texFillInfo, SDL_Texture* texX, SDL_Texture* texO, int board[BOARD_ROWS][BOARD_COLS], TTF_Font* font64,
    TTF_Font* font36, std::string nameP1, std::string nameP2, int currentPlayer, int matchTimeInSeconds, int countdownTime,
    int controlMode, int cursorRow, int cursorCol)
{
    // 1-4. VẼ ẢNH NHƯ CŨ (Bạn giữ nguyên đoạn code vẽ BG, UI, Board, và vòng lặp vẽ X, O nhé)
    DrawTexture(bgPlaying, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DrawTexture(texInfoX, renderer, 42, 136, 460, 883);
    DrawTexture(texInfoO, renderer, 1418, 136, 460, 883);
    DrawTexture(texScoreboard, renderer, 441, 16, 1042, 194);
    DrawTexture(texBoard, renderer, BOARD_START_X, BOARD_START_Y, 840, 840);
    DrawTexture(texFillInfo, renderer, 73, 442, 395, 310);
    DrawTexture(texFillInfo, renderer, 1447, 442, 395, 310);

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (board[r][c] != 0) {
                // Tính tọa độ pixel của ô hiện tại
                int drawX = BOARD_START_X + (c * CELL_SIZE);
                int drawY = BOARD_START_Y + (r * CELL_SIZE);

                // Vẽ quân cờ lùi vào 4px để nằm lọt giữa ô (kích thước 62x62)
                if (board[r][c] == 1) {
                    DrawTexture(texX, renderer, drawX + 4, drawY + 4, 62, 62);
                }
                else if (board[r][c] == 2) {
                    DrawTexture(texO, renderer, drawX + 4, drawY + 4, 62, 62);
                }
            }
        }
    }

    SDL_Color colorRed = { 255, 0, 0, 255 };
    SDL_Color colorBlue = { 0, 0, 255, 255 };
    SDL_Color colorWhite = { 255, 255, 255, 255 };
    SDL_Color colorGreen = { 0, 255, 0, 255 };

    // Tạo dấu ba chấm động theo thời gian hệ thống (nhảy mỗi nửa giây)
    int dotCount = (SDL_GetTicks() / 500) % 4; // Ra kết quả: 0, 1, 2, 3
    std::string dots = "";
    for (int i = 0; i < dotCount; i++) dots += ".";

    // --- 1. THỜI GIAN TRẬN ĐẤU (BẢNG TỈ SỐ) ---
    int minutes = matchTimeInSeconds / 60;
    int seconds = matchTimeInSeconds % 60;
    std::stringstream timeStr;
    timeStr << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    DrawTextCenter(renderer, font64, timeStr.str(), 955, 37, colorWhite);

    // --- 2. TÊN TRÊN BẢNG TỈ SỐ ---
    DrawTextCenter(renderer, font64, nameP1 == "" ? "Player 1" : nameP1, 751, 117, colorWhite);
    DrawTextCenter(renderer, font64, nameP2 == "" ? "Player 2" : nameP2, 1172, 117, colorWhite);

    // --- 3. TÊN TRÊN KHUNG THÔNG TIN ---
    DrawTextCenter(renderer, font64, nameP1 == "" ? "Player 1" : nameP1, 271, 473, colorRed);
    DrawTextCenter(renderer, font64, nameP2 == "" ? "Player 2" : nameP2, 1644, 473, colorBlue);

    // --- 4. TRẠNG THÁI & ĐỒNG HỒ ---
    SDL_Color timeColor = (countdownTime <= 5) ? colorRed : colorWhite;
    std::string timeSecStr = std::to_string(countdownTime) + "s";

    if (currentPlayer == 1) {
        // P1 Đang đánh
        DrawTextCenter(renderer, font36, "Simulating Outcome" + dots, 272, 558, colorGreen);
        DrawTextCenter(renderer, font64, "TIME", 270, 620, colorWhite);
        DrawTextCenter(renderer, font64, timeSecStr, 270, 680, timeColor);

        // P2 Chờ
        DrawTextCenter(renderer, font36, "Awaiting Challenger...", 1647, 558, colorWhite);
        DrawTextCenter(renderer, font64, "TIME", 1644, 620, colorWhite);
        // Có thể để trống hoặc để "Wait" thay vì hiện giây cho người chờ
        DrawTextCenter(renderer, font64, "--", 1644, 680, colorWhite);
    }
    else {
        // P1 Chờ
        DrawTextCenter(renderer, font36, "Awaiting Challenger...", 272, 558, colorWhite);
        DrawTextCenter(renderer, font64, "TIME", 270, 620, colorWhite);
        DrawTextCenter(renderer, font64, "--", 270, 680, colorWhite);

        // P2 Đang đánh
        DrawTextCenter(renderer, font36, "Simulating Outcome" + dots, 1647, 558, colorGreen);
        DrawTextCenter(renderer, font64, "TIME", 1644, 620, colorWhite);
        DrawTextCenter(renderer, font64, timeSecStr, 1644, 680, timeColor);
    }
    // =========================================================
    // VẼ CON TRỎ BÀN PHÍM (CHỈ HIỆN KHI CONTROL MODE == 1)
    // =========================================================
    if (controlMode == 1) {
        // TÍNH TOÁN ĐUẨN XÁC THEO THÔNG SỐ CONSTANTS CỦA KIN
        SDL_Rect cursorRect = {
            BOARD_START_X + (cursorCol * CELL_SIZE),
            BOARD_START_Y + (cursorRow * CELL_SIZE),
            CELL_SIZE,
            CELL_SIZE
        };

        // Đặt bút màu Đỏ (R=255, G=0, B=0)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Vẽ viền chữ nhật đỏ nhấp nháy
        SDL_RenderDrawRect(renderer, &cursorRect);

        // Mẹo UI: Vẽ thêm 2 vòng nữa thụt vào trong để nét khung đỏ dày 3 pixels
        cursorRect.x += 1; cursorRect.y += 1; cursorRect.w -= 2; cursorRect.h -= 2;
        SDL_RenderDrawRect(renderer, &cursorRect);

        cursorRect.x += 1; cursorRect.y += 1; cursorRect.w -= 2; cursorRect.h -= 2;
        SDL_RenderDrawRect(renderer, &cursorRect);
    }
}

int HandlePlayClick(int mouseX, int mouseY, int board[BOARD_ROWS][BOARD_COLS], int* currentPlayer, int currentState, Uint32* turnStartTime) {
    int newState = currentState;
    if (mouseX >= BOARD_START_X && mouseX <= BOARD_START_X + (BOARD_COLS * CELL_SIZE) &&
        mouseY >= BOARD_START_Y && mouseY <= BOARD_START_Y + (BOARD_ROWS * CELL_SIZE)) {

        int clickedCol = (mouseX - BOARD_START_X) / CELL_SIZE;
        int clickedRow = (mouseY - BOARD_START_Y) / CELL_SIZE;

        if (clickedRow >= 0 && clickedRow < BOARD_ROWS && clickedCol >= 0 && clickedCol < BOARD_COLS) {
            if (board[clickedRow][clickedCol] == 0) {
                board[clickedRow][clickedCol] = *currentPlayer;
                *currentPlayer = (*currentPlayer == 1) ? 2 : 1;

                // Đánh xong thì reset lại đồng hồ đếm ngược cho người kia
                *turnStartTime = SDL_GetTicks();
            }
        }
    }
    return newState;
}