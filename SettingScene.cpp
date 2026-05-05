#include "SettingScene.h"
#include "TextureManager.h"

void RenderSettingScene(SDL_Renderer* renderer, SDL_Texture* bgSetting, SDL_Texture* texPanel, SDL_Texture* texVanDijk,
    SDL_Texture* btnSound, SDL_Texture* btnControl, SDL_Texture* btnRules, SDL_Texture* btnAbout,
    SDL_Texture* texLoa, SDL_Texture* texSFXIcon, SDL_Texture* texTrack1, SDL_Texture* texBG1, SDL_Texture* texFill1, SDL_Texture* texKnob1,
    SDL_Texture* texTrack2, SDL_Texture* texFill2, SDL_Texture* texKnob2,
    SDL_Texture* texPanelControl, SDL_Texture* texBall1, SDL_Texture* texBall2, // <--- THÊM 2 ẢNH QUẢ BÓNG
    SDL_Texture* texClose,
    int currentTab, int musicVolume, int sfxVolume, int controlMode, // <--- THÊM BIẾN controlMode
    SDL_Texture* texRules, SDL_Texture* texAbout,
    bool hoverSound, bool hoverControl, bool hoverRules, bool hoverAbout,
    bool hoverBall1, bool hoverBall2,
    bool hoverClose)
{
    // 1. Vẽ Nền và Van Dijk
    DrawTexture(bgSetting, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DrawTexture(texVanDijk, renderer, 1507, 101, 413, 777);

    // 2. Vẽ Panel tổng
    DrawTexture(texPanel, renderer, 398, 184, 1118, 694);

    // Mảng chứa thông tin để dùng vòng lặp cho gọn (hoặc vẽ rời như cũ cũng được)
    SDL_Texture* tabs[] = { btnSound, btnControl, btnRules, btnAbout };
    bool hovers[] = { hoverSound, hoverControl, hoverRules, hoverAbout };
    int tabX[] = { 398, 688, 960, 1247 };

    for (int i = 0; i < 4; i++) {
        // MẶC ĐỊNH: Reset màu về bình thường
        SDL_SetTextureColorMod(tabs[i], 255, 255, 255);

        // 1. XỬ LÝ HOVER: Nếu di chuột qua thì làm nút sáng lên (hơi ám trắng)
        if (hovers[i]) {
            SDL_SetTextureColorMod(tabs[i], 200, 200, 200); // Làm tối/sáng tùy ý
        }

        // 2. XỬ LÝ ACTIVE: Nếu là Tab đang chọn, có thể vẽ nút to hơn hoặc làm nó rực rỡ hơn
        SDL_Rect tabRect = { tabX[i], 81, 269, 103 };
        if (i == currentTab) {
            SDL_SetTextureColorMod(tabs[i], 255, 255, 255); // Giữ nguyên màu thật
            tabRect.y -= 10; // Đẩy nút cao lên một chút để báo hiệu đang chọn
        }
        else {
            // Nút không được chọn thì cho nó hơi tối lại một chút để làm nền
            if (!hovers[i]) SDL_SetTextureColorMod(tabs[i], 150, 150, 150);
        }

        SDL_RenderCopy(renderer, tabs[i], NULL, &tabRect);
    }

    // 3. Vẽ 4 nút Tab
    DrawTexture(btnSound, renderer, 398, 81, 269, 103);
    DrawTexture(btnControl, renderer, 688, 81, 269, 103);
    DrawTexture(btnRules, renderer, 960, 81, 269, 103);
    DrawTexture(btnAbout, renderer, 1247, 81, 269, 103);

    // 4. HIỂN THỊ NỘI DUNG THEO TAB
    if (currentTab == 0) {
        DrawTexture(texLoa, renderer, 422, 327, 193, 202);
        DrawTexture(texSFXIcon, renderer, 422, 612, 193, 203);

        // --- SLIDER 1: MUSIC ---
        DrawTexture(texBG1, renderer, 657, 399, 816, 58);
        DrawTexture(texTrack1, renderer, 646, 399, 844, 58);

        // TOÁN HỌC MỚI: Quãng đường = TrackW (816) - KnobW (154) = 662
        int maxTravel1 = 816 - 154;
        int knob1X = 657 + (int)((musicVolume / 100.0f) * maxTravel1);

        // Chiều dài thanh màu = Tọa độ tâm của cục Knob - Tọa độ gốc Track
        int fill1Width = (knob1X + (154 / 2)) - 657;
        DrawTexture(texFill1, renderer, 657, 409, fill1Width, 37);
        DrawTexture(texKnob1, renderer, knob1X, 386, 154, 83);

        // --- SLIDER 2: SFX ---
        DrawTexture(texTrack2, renderer, 646, 685, 844, 58);

        // TOÁN HỌC MỚI: Quãng đường = TrackW (816) - KnobW (115) = 701
        int maxTravel2 = 816 - 115;
        int knob2X = 657 + (int)((sfxVolume / 100.0f) * maxTravel2);

        // Chiều dài thanh màu = Tọa độ tâm của cục Knob - Tọa độ gốc Track
        int fill2Width = (knob2X + (115 / 2)) - 657;
        DrawTexture(texFill2, renderer, 657, 703, fill2Width, 23);
        DrawTexture(texKnob2, renderer, knob2X, 656, 115, 115);
    }
    else if (currentTab == 1) {
        // --- TAB CONTROL ---

        DrawTexture(texPanelControl, renderer, 372, 184, 1138, 694);

        // Quả bóng 1 (Bàn phím)
        SDL_Rect rectB1 = { 489, 222, 218, 218 };
        if (controlMode != 1) SDL_SetTextureColorMod(texBall1, 80, 80, 80); // Nếu ko chọn thì làm tối
        else SDL_SetTextureColorMod(texBall1, 255, 255, 255);

        // Hiệu ứng Hover phóng to bóng
        if (hoverBall1) { rectB1.x -= 10; rectB1.y -= 10; rectB1.w += 20; rectB1.h += 20; }
        SDL_RenderCopy(renderer, texBall1, NULL, &rectB1);

        // Quả bóng 2 (Chuột)
        SDL_Rect rectB2 = { 1169, 222, 213, 218 };
        if (controlMode != 0) SDL_SetTextureColorMod(texBall2, 80, 80, 80);
        else SDL_SetTextureColorMod(texBall2, 255, 255, 255);

        if (hoverBall2) { rectB2.x -= 10; rectB2.y -= 10; rectB2.w += 20; rectB2.h += 20; }
        SDL_RenderCopy(renderer, texBall2, NULL, &rectB2);
    }
    else if (currentTab == 2) {
        // --- TAB RULES ---
        // Chỉ cần 1 dòng duy nhất để in tấm ảnh luật chơi theo đúng tọa độ Kin đưa
        DrawTexture(texRules, renderer, 516, 270, 887, 521);
    }
    else if (currentTab == 3) {
        DrawTexture(texAbout, renderer, 453, 208, 1050, 646);
    }

    // VẼ NÚT CLOSE (X) Ở GÓC PHẢI MÀN HÌNH
    // ==========================================
    SDL_Rect rectClose = { 1813, 10, 89, 89 };

    if (hoverClose) {
        // Hiệu ứng Hover: Phóng to nút lên một chút khi rê chuột vào cho "Đã"
        rectClose.x -= 5; rectClose.y -= 5;
        rectClose.w += 10; rectClose.h += 10;
        SDL_SetTextureColorMod(texClose, 255, 200, 200); // Ám chút màu đỏ 
    }
    else {
        SDL_SetTextureColorMod(texClose, 255, 255, 255); // Bình thường
    }

    SDL_RenderCopy(renderer, texClose, NULL, &rectClose);
}

// XỬ LÝ SỰ KIỆN KÉO THẢ SLIDER
void HandleSettingEvents(SDL_Event& event, int* currentTab, int* musicVolume, int* sfxVolume, int* controlMode,
    bool* isDraggingMusic, bool* isDraggingSFX, int& currentState)
{
    int mouseX = event.button.x;
    int mouseY = event.button.y;

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

        // --- MỚI THÊM: CLICK NÚT CLOSE ---
        if (mouseX >= 1813 && mouseX <= 1813 + 89 && mouseY >= 10 && mouseY <= 10 + 89) {
            currentState = STATE_MENU; // Quay xe về màn hình Menu ngay lập tức!
            return; // Thoát hàm để tránh click nhầm các nút bên dưới
        }

        // 1. Chuyển Tab (Đã cập nhật Y = 81 đến 184 cho khớp với ảnh vẽ)
        if (mouseY >= 81 && mouseY <= 184) {
            if (mouseX >= 398 && mouseX <= 667) *currentTab = 0; // Sound
            else if (mouseX >= 688 && mouseX <= 957) *currentTab = 1; // Control
            else if (mouseX >= 960 && mouseX <= 1229) *currentTab = 2; // Rules
            else if (mouseX >= 1247 && mouseX <= 1516) *currentTab = 3; // About
        }

        // 2. Bắt đầu Kéo Slider (Chỉ xử lý khi ở Tab Sound)
        if (*currentTab == 0) {
            // --- TÍNH TOÁN VỊ TRÍ HIỆN TẠI CỦA 2 CÁI NÚT KNOB ---
            int fill1Width = (int)((*musicVolume / 100.0f) * 816);
            int knob1X = 657 + fill1Width - (154 / 2); // Tọa độ X thực tế của Knob 1

            int fill2Width = (int)((*sfxVolume / 100.0f) * 816);
            int knob2X = 657 + fill2Width - (115 / 2); // Tọa độ X thực tế của Knob 2

            // --- KIỂM TRA CLICK CHÍNH XÁC VÀO NÚT ---

            // Click vào đúng cục Knob 1 (Music): X từ knob1X đến knob1X + 154
            if (mouseX >= knob1X && mouseX <= knob1X + 154 && mouseY >= 386 && mouseY <= 386 + 83) {
                *isDraggingMusic = true;
            }
            // Click vào đúng cục Knob 2 (SFX): X từ knob2X đến knob2X + 115
            else if (mouseX >= knob2X && mouseX <= knob2X + 115 && mouseY >= 656 && mouseY <= 656 + 115) {
                *isDraggingSFX = true;
            }
        }
        else if (*currentTab == 1) {
            // Click Bóng 1 (Phím)
            if (mouseX >= 489 && mouseX <= 489 + 218 && mouseY >= 222 && mouseY <= 222 + 218) {
                *controlMode = 1;
            }
            // Click Bóng 2 (Chuột)
            else if (mouseX >= 1169 && mouseX <= 1169 + 213 && mouseY >= 222 && mouseY <= 222 + 218) {
                *controlMode = 0;
            }
        }
    }

    // 3. Nhả chuột ra -> Ngừng kéo
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        *isDraggingMusic = false;
        *isDraggingSFX = false;
    }

    // 4. Cập nhật Âm lượng khi đang giữ chuột và rê đi
    // 4. Cập nhật Âm lượng khi đang giữ chuột và rê đi
    else if (event.type == SDL_MOUSEMOTION) {
        mouseX = event.motion.x;

        if (*isDraggingMusic) {
            // maxTravel = 662. Tâm bắt đầu kéo là 657 + (154/2) = 734
            float percent = (mouseX - 734.0f) / 662.0f;
            *musicVolume = (int)(percent * 100);

            if (*musicVolume < 0) *musicVolume = 0;
            if (*musicVolume > 100) *musicVolume = 100;
        }
        else if (*isDraggingSFX) {
            // maxTravel = 701. Tâm bắt đầu kéo là 657 + (115/2) = 714
            float percent = (mouseX - 714.0f) / 701.0f;
            *sfxVolume = (int)(percent * 100);

            if (*sfxVolume < 0) *sfxVolume = 0;
            if (*sfxVolume > 100) *sfxVolume = 100;
        }
    }
}