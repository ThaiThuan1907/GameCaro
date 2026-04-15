#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "TextureManager.h"
#include "Constants.h"
#include "UIButton.h"
#include "MenuScene.h"
#include "PlayScene.h" 
#include "NewGameScene.h"
#include "PvAIScene.h"
#include "PvPScene.h"
#include "UITextBox.h"

int main(int argc, char* args[])
{
    // --- KHỞI TẠO BIẾN AN TOÀN ---
    SDL_Window* gameWindow = NULL;
    SDL_Renderer* gameRenderer = NULL;
    bool isRunning = false;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Loi khoi tao SDL: " << SDL_GetError() << "\n";
        return -1;
    }
    if (TTF_Init() < 0) {
        std::cout << "Loi khoi tao TTF: " << TTF_GetError() << "\n";
        return -1;
    }

    gameWindow = SDL_CreateWindow("Đồ Án Caro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(gameRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    int currentState = STATE_MENU;
    isRunning = true;
    int currentMouseX = 0;
    int currentMouseY = 0;

    TTF_Font* font123 = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 123); // Cho tiêu đề PVP
    TTF_Font* font64 = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 64);   // Cho Tên, Thời gian
    TTF_Font* font48 = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 48);   // Cho Nút bấm
    TTF_Font* font36 = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 36);   // Cho dòng trạng thái

    // --- BIẾN DÀNH CHO IN-GAME ---
    int gameBoard[BOARD_ROWS][BOARD_COLS] = { 0 }; // Lưới cờ 12x12
    int currentPlayer = 1;                         // 1 là X đi trước, 2 là O
    Uint32 matchStartTime = 0;
    Uint32 turnStartTime = 0;
    int turnLimit = 15; // Mỗi lượt có 15 giây để đánh

    // --- 1. TẢI ẢNH CHO MENU ---
    SDL_Texture* bgMenu = LoadTexture("Assets/Pictures/Menu/Menu1.png", gameRenderer);
    SDL_Texture* button_NewGame = LoadTexture("Assets/Pictures/Menu/button_NewGame.png", gameRenderer);
    SDL_Texture* button_LoadGame = LoadTexture("Assets/Pictures/Menu/button_LoadGame.png", gameRenderer);
    SDL_Texture* button_Setting = LoadTexture("Assets/Pictures/Menu/button_Setting.png", gameRenderer);
    SDL_Texture* button_Exit = LoadTexture("Assets/Pictures/Menu/button_Exit.png", gameRenderer);

    UIButton btnNewGame = CreateButton(768, 482, 385, 133, button_NewGame);
    UIButton btnLoadGame = CreateButton(768, 622, 385, 133, button_LoadGame);
    UIButton btnSetting = CreateButton(768, 762, 385, 133, button_Setting);
    UIButton btnExit = CreateButton(768, 902, 385, 133, button_Exit);

    // --- 2. TẢI ẢNH CHO NEW GAME ---
    SDL_Texture* bgNewGame = LoadTexture("Assets/Pictures/Menu/New Game/BG.png", gameRenderer);
    SDL_Texture* button_PvP = LoadTexture("Assets/Pictures/Menu/New Game/PVP.png", gameRenderer);
    SDL_Texture* button_PvAI = LoadTexture("Assets/Pictures/Menu/New Game/PVAI.png", gameRenderer);

    UIButton btnPvAI = CreateButton(665, 456, 271, 103, button_PvAI);
    UIButton btnPvP = CreateButton(1044, 456, 228, 118, button_PvP);

    // --- 3. TẢI ẢNH CHO CHỌN ĐỘ KHÓ ---
    SDL_Texture* button_Easy = LoadTexture("Assets/Pictures/Menu/New Game/Menu NewGame/PVAI/Easy.png", gameRenderer);
    SDL_Texture* button_Medium = LoadTexture("Assets/Pictures/Menu/New Game/Menu NewGame/PVAI/Medium.png", gameRenderer);
    SDL_Texture* button_Hard = LoadTexture("Assets/Pictures/Menu/New Game/Menu NewGame/PVAI/Hard.png", gameRenderer);

    UIButton btnEasy = CreateButton(837, 337, 246, 119, button_Easy);
    UIButton btnMedium = CreateButton(775, 476, 408, 98, button_Medium);
    UIButton btnHard = CreateButton(837, 615, 246, 90, button_Hard);

    // --- 4. KHỞI TẠO FONT VÀ NẶN CHỮ CHO PVP ---

    // Load 2 cỡ chữ: Size 123 cho Tiêu đề, Size 48 cho các thành phần còn lại
    TTF_Font* fontTitle = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 123);
    TTF_Font* fontNormal = TTF_OpenFont("Assets/Fonts/VT323/VT323-Regular.ttf", 48);

    // 1. Nặn chữ Tiêu đề "PVP" (Màu đen, Size 123)
    SDL_Surface* surfTitle = TTF_RenderText_Solid(fontTitle, "PVP", { 0, 0, 0, 255 });
    SDL_Texture* texTitle = SDL_CreateTextureFromSurface(gameRenderer, surfTitle);
    UIButton lblTitle = CreateButton(897, 314, surfTitle->w, surfTitle->h, texTitle);
    SDL_FreeSurface(surfTitle);

    // 2. Tạo 2 khung nhập tên P1, P2 (Đúng tọa độ và size Figma)
    UITextBox boxPlayer1 = CreateTextBox(873, 483, 350, 70);
    UITextBox boxPlayer2 = CreateTextBox(873, 576, 350, 70);

    // 3. Nặn nút START (Màu Đỏ, Size 48)
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontNormal, "START", { 255, 0, 0, 255 });
    SDL_Texture* textBtnTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);
    UIButton btnStartGame = CreateButton(911, 680, textSurface->w, textSurface->h, textBtnTexture);
    SDL_FreeSurface(textSurface);

    // 4. Nặn các dòng chữ trang trí (Không cần ép kiểu hay nhân 0.8 nữa vì đã đúng size 48)
    SDL_Surface* surfInst = TTF_RenderText_Solid(fontNormal, "Enter Player Names", { 0, 0, 0, 255 });
    SDL_Texture* texInst = SDL_CreateTextureFromSurface(gameRenderer, surfInst);
    UIButton lblInstruction = CreateButton(783, 423, surfInst->w, surfInst->h, texInst);
    SDL_FreeSurface(surfInst);

    SDL_Surface* surfP1 = TTF_RenderText_Solid(fontNormal, "Player 1:", { 200, 0, 0, 255 });
    SDL_Texture* texP1 = SDL_CreateTextureFromSurface(gameRenderer, surfP1);
    UIButton lblP1 = CreateButton(689, 493, surfP1->w, surfP1->h, texP1);
    SDL_FreeSurface(surfP1);

    SDL_Surface* surfP2 = TTF_RenderText_Solid(fontNormal, "Player 2:", { 0, 0, 200, 255 });
    SDL_Texture* texP2 = SDL_CreateTextureFromSurface(gameRenderer, surfP2);
    UIButton lblP2 = CreateButton(689, 587, surfP2->w, surfP2->h, texP2);
    SDL_FreeSurface(surfP2);



    // --- 5. TẢI ẢNH CHO IN GAME ---

    SDL_Texture* bgPlaying = LoadTexture("Assets/Pictures/Menu/In Game/BG In Game.png", gameRenderer);
    SDL_Texture* texBoard = LoadTexture("Assets/Pictures/Menu/In Game/Board.png", gameRenderer);      // Bàn cờ cỏ
    SDL_Texture* texScoreboard = LoadTexture("Assets/Pictures/Menu/In Game/ScoreBoard.png", gameRenderer);
    SDL_Texture* texInfoX = LoadTexture("Assets/Pictures/Menu/In Game/InfoX.png", gameRenderer);
    SDL_Texture* texInfoO = LoadTexture("Assets/Pictures/Menu/In Game/InfoO.png", gameRenderer);
    SDL_Texture* texX = LoadTexture("Assets/Pictures/Menu/In Game/X.png", gameRenderer);              // Quân X
    SDL_Texture* texO = LoadTexture("Assets/Pictures/Menu/In Game/O.png", gameRenderer);              // Quân O
    SDL_Texture* texFillInfo = LoadTexture("Assets/Pictures/Menu/In Game/RecDisplay.png", gameRenderer);

    // ================= VÒNG LẶP TRÒ CHƠI =================
    while (isRunning == true) {

        // --- PHẦN 1: BẮT TÍN HIỆU NGƯỜI DÙNG ---
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                currentMouseX = event.motion.x;
                currentMouseY = event.motion.y;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int clickX = event.button.x;
                    int clickY = event.button.y;

                    if (currentState == STATE_MENU) {
                        currentState = HandleMenuClick(clickX, clickY, btnNewGame, btnLoadGame, btnSetting, btnExit, currentState);
                        if (currentState == -1) {
                            isRunning = false;
                        }
                    }
                    else if (currentState == STATE_NEW_GAME_SELECT) {
                        currentState = HandleNewGameClick(clickX, clickY, btnPvAI, btnPvP, currentState);
                    }
                    else if (currentState == STATE_PVAI_DIFFICULTY) {
                        currentState = HandlePvAIClick(clickX, clickY, btnEasy, btnMedium, btnHard, currentState);
                    }
                    else if (currentState == STATE_PVP_INPUT_NAME) {
                        currentState = HandlePvPMouseClick(clickX, clickY, &boxPlayer1, &boxPlayer2, btnStartGame, currentState);
                    }
                    else if (currentState == STATE_PLAYING) {
                        currentState = HandlePlayClick(clickX, clickY, gameBoard, &currentPlayer, currentState, &turnStartTime);
                    }

                }
            }
            // SỰ KIỆN GÕ PHÍM PHẢI NẰM TRONG VÒNG LẶP NÀY
            else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN) {
                if (currentState == STATE_PVP_INPUT_NAME) {
                    HandleTextBoxEvent(&boxPlayer1, event);
                    HandleTextBoxEvent(&boxPlayer2, event);
                }
            }
        } // <--- ĐÓNG VÒNG LẶP SỰ KIỆN TẠI ĐÂY


        // --- PHẦN 2: CẬP NHẬT TRẠNG THÁI HOVER CHO NÚT ---
        if (currentState == STATE_MENU) {
            btnNewGame.isHovered = CheckMouseHover(btnNewGame, currentMouseX, currentMouseY);
            btnLoadGame.isHovered = CheckMouseHover(btnLoadGame, currentMouseX, currentMouseY);
            btnSetting.isHovered = CheckMouseHover(btnSetting, currentMouseX, currentMouseY);
            btnExit.isHovered = CheckMouseHover(btnExit, currentMouseX, currentMouseY);
        }
        else if (currentState == STATE_NEW_GAME_SELECT) {
            btnPvAI.isHovered = CheckMouseHover(btnPvAI, currentMouseX, currentMouseY);
            btnPvP.isHovered = CheckMouseHover(btnPvP, currentMouseX, currentMouseY);
        }
        else if (currentState == STATE_PVAI_DIFFICULTY) {
            btnEasy.isHovered = CheckMouseHover(btnEasy, currentMouseX, currentMouseY);
            btnMedium.isHovered = CheckMouseHover(btnMedium, currentMouseX, currentMouseY);
            btnHard.isHovered = CheckMouseHover(btnHard, currentMouseX, currentMouseY);
        }
        else if (currentState == STATE_PVP_INPUT_NAME) {
            btnStartGame.isHovered = CheckMouseHover(btnStartGame, currentMouseX, currentMouseY);
        }

        // --- PHẦN 3: VẼ RA MÀN HÌNH ---
        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gameRenderer);

        switch (currentState) {
        case STATE_MENU:
            RenderMenuScene(gameRenderer, bgMenu, btnNewGame, btnLoadGame, btnSetting, btnExit);
            break;

        case STATE_NEW_GAME_SELECT:
            RenderNewGameScene(gameRenderer, bgNewGame, btnPvAI, btnPvP);
            break;

        case STATE_PVAI_DIFFICULTY:
            RenderPvAIScene(gameRenderer, bgNewGame, btnEasy, btnMedium, btnHard);
            break;

        case STATE_PVP_INPUT_NAME:
            RenderPvPScene(gameRenderer, bgNewGame, lblTitle, boxPlayer1, boxPlayer2, btnStartGame, fontNormal, lblInstruction, lblP1, lblP2);
            break;

        case STATE_PLAYING:
            // Nếu vừa mới vào ván, bắt đầu bấm giờ!
            if (matchStartTime == 0) {
                matchStartTime = SDL_GetTicks();
                turnStartTime = SDL_GetTicks();
            }

            // Tính toán thời gian thực tại frame hiện tại
            {
                Uint32 currentTicks = SDL_GetTicks();
                int matchTime = (currentTicks - matchStartTime) / 1000;
                int turnTimeLeft = turnLimit - ((currentTicks - turnStartTime) / 1000);

                // Nếu hết 15s mà chưa đánh -> tự động đổi lượt (Tùy chọn nâng cao)
                if (turnTimeLeft < 0) {
                    turnTimeLeft = 0;
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    turnStartTime = SDL_GetTicks(); // Reset đồng hồ
                }

                // Gọi hàm vẽ siêu cấp: Truyền tên (từ 2 cái Textbox lúc nãy) và thời gian vào
                RenderPlayScene(gameRenderer, bgPlaying, texBoard, texScoreboard, texInfoX, texInfoO, texFillInfo, texX, texO, gameBoard, font64, font36, boxPlayer1.textContent, boxPlayer2.textContent, currentPlayer, matchTime, turnTimeLeft);
            }
            break;
        } // <--- ĐÓNG KHỐI SWITCH TẠI ĐÂY


        SDL_RenderPresent(gameRenderer);

    } // <--- ĐÓNG VÒNG LẶP CHÍNH isRunning TẠI ĐÂY


    // --- DỌN DẸP BỘ NHỚ TRƯỚC KHI TẮT ---
    SDL_DestroyTexture(bgMenu);
    SDL_DestroyTexture(button_NewGame);
    SDL_DestroyTexture(button_LoadGame);
    SDL_DestroyTexture(button_Setting);
    SDL_DestroyTexture(button_Exit);

    SDL_DestroyTexture(bgNewGame);
    SDL_DestroyTexture(button_PvAI);
    SDL_DestroyTexture(button_PvP);

    SDL_DestroyTexture(button_Easy);
    SDL_DestroyTexture(button_Medium);
    SDL_DestroyTexture(button_Hard);

    SDL_DestroyTexture(textBtnTexture);
    SDL_DestroyTexture(texInst);
    SDL_DestroyTexture(texP1);
    SDL_DestroyTexture(texP2);

    SDL_DestroyTexture(texTitle);
    TTF_CloseFont(fontTitle);
    TTF_CloseFont(fontNormal);
    TTF_CloseFont(font64);
    TTF_CloseFont(font36);
    TTF_Quit();

    SDL_DestroyTexture(bgPlaying);
    SDL_DestroyTexture(texBoard);
    SDL_DestroyTexture(texScoreboard);
    SDL_DestroyTexture(texInfoX);
    SDL_DestroyTexture(texInfoO);
    SDL_DestroyTexture(texX);
    SDL_DestroyTexture(texO);

    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();

    return 0;
}