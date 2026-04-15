#pragma once

// Quy t?c 4: H?ng s? vi?t hoa toàn b? [cite: 104-106]
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// --- THÔNG SỐ BÀN CỜ 12x12 ---
const int BOARD_ROWS = 12;
const int BOARD_COLS = 12;
const int CELL_SIZE = 70;
const int BOARD_START_X = 540;
const int BOARD_START_Y = 221;

enum GameState {
    STATE_MENU,
    STATE_NEW_GAME_SELECT, // Ch?n PvAI ho?c PvP 
    STATE_PVAI_DIFFICULTY,  // Ch?n Ez, Medium, Hard 
    STATE_PVP_INPUT_NAME,   // Nh?p tên P1, P2 [cite: 11]
    STATE_LOAD_GAME,        // Màn hình Load file [cite: 13]
    STATE_SETTINGS_SOUND,   // Cài ??t âm thanh [cite: 19]
    STATE_SETTINGS_CONTROL, // Cài ??t ?i?u khi?n [cite: 20]
    STATE_SETTINGS_RULES,   // Xem lu?t ch?i [cite: 23]
    STATE_SETTINGS_ABOUT,   // Xem thông tin [cite: 23]
    STATE_PLAYING           // ?ang trong tr?n ??u [cite: 24]
};

enum AIDifficulty {
    AI_EASY = 0,
    AI_MEDIUM = 1,
    AI_HARD = 2
};