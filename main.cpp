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
#include "FileManager.h"
#include "SettingScene.h"
#include "LoadGameScene.h"

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

    // --- BIẾN DÀNH CHO LOAD GAME ---
    std::vector<std::string> saveFiles;
    int selectedSaveIndex = -1;
    int currentScroll = 0;
    UITextBox searchBox = CreateTextBox(411, 125, 1200, 78);

    // --- BIẾN DÀNH CHO SETTING ---
    int currentSettingTab = 0;   // 0: Sound, 1: Control, 2: Rules, 3: About
    int musicVolume = 50;        // Âm lượng nhạc nền 0-100%
    int sfxVolume = 80;          // Âm lượng hiệu ứng 0-100%
    bool isDraggingMusic = false; // Trạng thái giữ chuột kéo thanh Music
    bool isDraggingSFX = false;   // Trạng thái giữ chuột kéo thanh SFX
    int controlMode = 0;         // 0: Đánh bằng Chuột, 1: Đánh bằng Bàn phím

    // --- BIẾN CHO CON TRỎ BÀN PHÍM ---
    int cursorRow = BOARD_ROWS / 2;
    int cursorCol = BOARD_COLS / 2;

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

    // --- 6. TẢI ẢNH CHO LOAD GAME ---
    SDL_Texture* bgLoadGame = LoadTexture("Assets/Pictures/Menu/Load Game/BG.png", gameRenderer);
    SDL_Texture* texSearchBar = LoadTexture("Assets/Pictures/Menu/Load Game/searchBar.png", gameRenderer);
    SDL_Texture* texPanel = LoadTexture("Assets/Pictures/Menu/Load Game/pannel.png", gameRenderer);
    SDL_Texture* imgBtnLoad = LoadTexture("Assets/Pictures/Menu/Load Game/btLoadGame.png", gameRenderer);
    SDL_Texture* imgBtnRename = LoadTexture("Assets/Pictures/Menu/Load Game/btRename.png", gameRenderer);
    SDL_Texture* imgBtnDelete = LoadTexture("Assets/Pictures/Menu/Load Game/btDelete.png", gameRenderer);

    UIButton btnLoad = CreateButton(1316, 351, 562, 198, imgBtnLoad);
    UIButton btnRename = CreateButton(1316, 570, 562, 198, imgBtnRename);
    UIButton btnDelete = CreateButton(1316, 789, 562, 198, imgBtnDelete);

    // --- 7. TẢI ẢNH CHO SETTING ---
    SDL_Texture* bgSetting = LoadTexture("Assets/Pictures/Menu/Setting/bg.png", gameRenderer);
    SDL_Texture* texPanelSetting = LoadTexture("Assets/Pictures/Menu/Setting/Menu.png", gameRenderer);
    SDL_Texture* texVanDijk = LoadTexture("Assets/Pictures/Menu/Setting/vanDijk.png", gameRenderer);

    SDL_Texture* btnSound = LoadTexture("Assets/Pictures/Menu/Setting/buttonSound.png", gameRenderer);
    SDL_Texture* btnControl = LoadTexture("Assets/Pictures/Menu/Setting/buttonControl.png", gameRenderer);
    SDL_Texture* btnRules = LoadTexture("Assets/Pictures/Menu/Setting/buttonRules.png", gameRenderer);
    SDL_Texture* btnAbout = LoadTexture("Assets/Pictures/Menu/Setting/buttonAbout.png", gameRenderer);

    SDL_Texture* texLoa = LoadTexture("Assets/Pictures/Menu/Setting/Sound/sound.png", gameRenderer);
    SDL_Texture* texSFXIcon = LoadTexture("Assets/Pictures/Menu/Setting/Sound/SFX.png", gameRenderer);

    SDL_Texture* texTrack1 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/Track.png", gameRenderer);
    SDL_Texture* texBG1 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/trackBG1.png", gameRenderer);
    SDL_Texture* texKnob1 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/knob1.png", gameRenderer);
    SDL_Texture* texFill1 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/fill1.png", gameRenderer);

    SDL_Texture* texTrack2 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/Track.png", gameRenderer);
    SDL_Texture* texKnob2 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/knob2.png", gameRenderer);
    SDL_Texture* texFill2 = LoadTexture("Assets/Pictures/Menu/Setting/Sound/fill2.png", gameRenderer);

    SDL_Texture* texPanelControl = LoadTexture("Assets/Pictures/Menu/Setting/Control/PanelControl.png", gameRenderer);
    SDL_Texture* texBall1 = LoadTexture("Assets/Pictures/Menu/Setting/Control/Ball1.png", gameRenderer);
    SDL_Texture* texBall2 = LoadTexture("Assets/Pictures/Menu/Setting/Control/Ball2.png", gameRenderer);
 
    SDL_Texture* texRules = LoadTexture("Assets/Pictures/Menu/Setting/Rule/Rule.png", gameRenderer);
    SDL_Texture* texAbout = LoadTexture("Assets/Pictures/Menu/Setting/About/aboutContent.png", gameRenderer);

    SDL_Texture* texClose = LoadTexture("Assets/Pictures/CloseButton.png", gameRenderer);
    SDL_Texture* texBack = LoadTexture("Assets/Pictures/BackButton.png", gameRenderer);

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

                // Nếu đang dùng chuột, cập nhật vị trí ô cờ để highlight
                if (controlMode == 0 && currentState == STATE_PLAYING) {
                    if (currentMouseX >= BOARD_START_X && currentMouseX < BOARD_START_X + (BOARD_COLS * CELL_SIZE) &&
                        currentMouseY >= BOARD_START_Y && currentMouseY < BOARD_START_Y + (BOARD_ROWS * CELL_SIZE)) {
                        cursorCol = (currentMouseX - BOARD_START_X) / CELL_SIZE;
                        cursorRow = (currentMouseY - BOARD_START_Y) / CELL_SIZE;
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int clickX = event.button.x;
                    int clickY = event.button.y;

                    if (currentState == STATE_MENU) {
                        currentState = HandleMenuClick(clickX, clickY, btnNewGame, btnLoadGame, btnSetting, btnExit, currentState);
                        if (currentState == STATE_LOAD_GAME) {
                            int dummyTime, loadedMode, loadedDiff;
                            if (FileManager::LoadGame("quicksave.sav", gameBoard, currentPlayer, dummyTime, loadedMode, loadedDiff)) {
                                SetGameMode(loadedMode, loadedDiff); 
                                currentState = STATE_PLAYING;
                                turnStartTime = SDL_GetTicks();
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Load Game", "Game loaded correctly!", NULL);
                            } else {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Load Game", "No save file!", NULL);
                                currentState = STATE_MENU;
                            }
                        }
                        if (currentState == -1) {
                            isRunning = false;
                        }
                    }
                    else if (currentState == STATE_NEW_GAME_SELECT) {
                        currentState = HandleNewGameClick(clickX, clickY, btnPvAI, btnPvP, currentState);
                    }
                    else if (currentState == STATE_PVAI_DIFFICULTY) {
                        currentState = HandlePvAIClick(clickX, clickY, btnEasy, btnMedium, btnHard, currentState);
                        if (currentState == STATE_PLAYING) {
                            ResetBoard(gameBoard); // DỌN DẸP BÀN CỜ [cite: 147]
                            matchStartTime = 0;
                        }
                    }
                    else if (currentState == STATE_PVP_INPUT_NAME) {
                        currentState = HandlePvPMouseClick(clickX, clickY, &boxPlayer1, &boxPlayer2, btnStartGame, currentState);
                        if (currentState == STATE_PLAYING) {
                            ResetBoard(gameBoard); // DỌN DẸP BÀN CỜ khi vào ván mới [cite: 147]
                            SetGameMode(0, 0); 
                            matchStartTime = 0; // Reset đồng hồ tổng
                        }
                    }
                    else if (currentState == STATE_PLAYING) {
                        currentState = HandlePlayClick(clickX, clickY, gameBoard, &currentPlayer, currentState, &turnStartTime);
                    }
                    else if (currentState == STATE_LOAD_GAME) {
                        currentState = HandleLoadGameClick(clickX, clickY, btnLoad, btnRename, btnDelete, &searchBox, saveFiles, &selectedSaveIndex, currentScroll, currentState);
                    }

                }
            }
            // SỰ KIỆN CUỘN CHUỘT
            else if (event.type == SDL_MOUSEWHEEL) {
                if (currentState == STATE_LOAD_GAME) {
                    if (event.wheel.y > 0) currentScroll -= 40;
                    else if (event.wheel.y < 0) currentScroll += 40;

                    if (currentScroll < 0) currentScroll = 0;
                    int maxScroll = ((int)saveFiles.size() * 80) - 701;
                    if (maxScroll < 0) maxScroll = 0;
                    if (currentScroll > maxScroll) currentScroll = maxScroll;
                }
            }
            // SỰ KIỆN GÕ PHÍM PHẢI NẰM TRONG VÒNG LẶP NÀY
            else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN) {
                if (currentState == STATE_PVP_INPUT_NAME) {
                    HandleTextBoxEvent(&boxPlayer1, event);
                    HandleTextBoxEvent(&boxPlayer2, event);
                }
                else if (currentState == STATE_LOAD_GAME) {
                    HandleTextBoxEvent(&searchBox, event);
                }
                else if (currentState == STATE_PLAYING) {
                    currentState = HandlePlayKeyboard(event, gameBoard, &currentPlayer, currentState, &turnStartTime, cursorRow, cursorCol);
                }
            }

            // XỬ LÝ SETTING
            if (currentState == STATE_SETTING) {
                HandleSettingEvents(event, &currentSettingTab, &musicVolume, &sfxVolume, &controlMode, &isDraggingMusic, &isDraggingSFX, currentState);
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
        else if (currentState == STATE_LOAD_GAME) {
            btnLoad.isHovered = CheckMouseHover(btnLoad, currentMouseX, currentMouseY);
            btnRename.isHovered = CheckMouseHover(btnRename, currentMouseX, currentMouseY);
            btnDelete.isHovered = CheckMouseHover(btnDelete, currentMouseX, currentMouseY);
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
        {
            // Nếu vừa mới vào ván, bắt đầu bấm giờ!
            if (matchStartTime == 0) {
                matchStartTime = SDL_GetTicks();
                turnStartTime = SDL_GetTicks();
            }

            Uint32 currentTicks = SDL_GetTicks();

                static int frozenMatchTime = 0;
                int matchTime = 0;
                if (!IsGameFinished()) {
                    matchTime = (currentTicks - matchStartTime) / 1000;
                    frozenMatchTime = matchTime;
                } else {
                    matchTime = frozenMatchTime;
                }
                
                int turnTimeLeft = turnLimit - ((currentTicks - turnStartTime) / 1000);

                // Nếu hết 15s mà chưa đánh -> tự động đổi lượt (Tùy chọn nâng cao)
                if (turnTimeLeft < 0) {
                    turnTimeLeft = 0;
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    turnStartTime = SDL_GetTicks(); // Reset đồng hồ
                }

                // Gọi bộ não AI thực hiện nước đi nếu đến lượt [cite: 153]
                CheckAIMove(gameBoard, &currentPlayer, &turnStartTime);

                // Gọi hàm vẽ siêu cấp: Truyền tên (từ 2 cái Textbox lúc nãy) và thời gian vào
                RenderPlayScene(gameRenderer, bgPlaying, texBoard, texScoreboard, texInfoX, texInfoO, texFillInfo, texX, texO, gameBoard, font64, font36, boxPlayer1.textContent, boxPlayer2.textContent, currentPlayer, matchTime, turnTimeLeft, controlMode, cursorRow, cursorCol);
            }
            break;

        case STATE_LOAD_GAME:
        {
            UIButton tmpClose = { 1813, 10, 89, 89 };
            bool hClose = CheckMouseHover(tmpClose, currentMouseX, currentMouseY);
            RenderLoadGameScene(gameRenderer, bgLoadGame, texSearchBar, texPanel, btnLoad, btnRename, btnDelete, &searchBox, saveFiles, selectedSaveIndex, currentScroll, font64, texClose, hClose);
            break;
        }

        case STATE_SETTING:
        {
            UIButton tmpSound = { 398, 81, 269, 103 };
            UIButton tmpControl = { 688, 81, 269, 103 };
            UIButton tmpRules = { 960, 81, 269, 103 };
            UIButton tmpAbout = { 1247, 81, 269, 103 };

            bool hSound = CheckMouseHover(tmpSound, currentMouseX, currentMouseY);
            bool hControl = CheckMouseHover(tmpControl, currentMouseX, currentMouseY);
            bool hRules = CheckMouseHover(tmpRules, currentMouseX, currentMouseY);
            bool hAbout = CheckMouseHover(tmpAbout, currentMouseX, currentMouseY);

            UIButton tmpBall1 = { 489, 222, 218, 218 };
            UIButton tmpBall2 = { 1169, 222, 213, 218 };
            bool hBall1 = CheckMouseHover(tmpBall1, currentMouseX, currentMouseY) && (currentSettingTab == 1);
            bool hBall2 = CheckMouseHover(tmpBall2, currentMouseX, currentMouseY) && (currentSettingTab == 1);

            UIButton tmpClose = { 1813, 10, 89, 89 };
            bool hClose = CheckMouseHover(tmpClose, currentMouseX, currentMouseY);

            RenderSettingScene(gameRenderer, bgSetting, texPanelSetting, texVanDijk,
                btnSound, btnControl, btnRules, btnAbout,
                texLoa, texSFXIcon, texTrack1, texBG1, texFill1, texKnob1,
                texTrack2, texFill2, texKnob2,
                texPanelControl, texBall1, texBall2,
                texClose,
                currentSettingTab, musicVolume, sfxVolume, controlMode,
                texRules, texAbout,
                hSound, hControl, hRules, hAbout,
                hBall1, hBall2,
                hClose);
            break;
        }
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
    SDL_DestroyTexture(texFillInfo);

    // Hủy Ảnh Load Game
    SDL_DestroyTexture(bgLoadGame);
    SDL_DestroyTexture(texSearchBar);
    SDL_DestroyTexture(texPanel);
    SDL_DestroyTexture(imgBtnLoad);
    SDL_DestroyTexture(imgBtnRename);
    SDL_DestroyTexture(imgBtnDelete);

    // Hủy Ảnh Setting
    SDL_DestroyTexture(bgSetting);
    SDL_DestroyTexture(texPanelSetting);
    SDL_DestroyTexture(texVanDijk);
    SDL_DestroyTexture(btnSound);
    SDL_DestroyTexture(btnControl);
    SDL_DestroyTexture(btnRules);
    SDL_DestroyTexture(btnAbout);
    SDL_DestroyTexture(texLoa);
    SDL_DestroyTexture(texSFXIcon);
    SDL_DestroyTexture(texTrack1);
    SDL_DestroyTexture(texBG1);
    SDL_DestroyTexture(texKnob1);
    SDL_DestroyTexture(texFill1);
    SDL_DestroyTexture(texTrack2);
    SDL_DestroyTexture(texKnob2);
    SDL_DestroyTexture(texFill2);
    SDL_DestroyTexture(texBall1);
    SDL_DestroyTexture(texBall2);
    SDL_DestroyTexture(texPanelControl);
    SDL_DestroyTexture(texRules);
    SDL_DestroyTexture(texAbout);
    SDL_DestroyTexture(texClose);
    SDL_DestroyTexture(texBack);
    SDL_DestroyTexture(texInfoO);
    SDL_DestroyTexture(texX);
    SDL_DestroyTexture(texO);

    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();

    return 0;
}