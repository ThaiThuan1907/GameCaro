#include "LoadGameScene.h"
#include "TextureManager.h"
#include "PlayScene.h" // Kế thừa hàm DrawTextCenter hoặc tự viết DrawTextLeft

// Hàm phụ trợ: In chữ căn TRÁI (Dùng cho thanh Search và Danh sách File)
void DrawTextLeft(SDL_Renderer* renderer, TTF_Font* font, std::string text, int x, int y, SDL_Color color) {
    if (text == "" || font == NULL) return;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void RenderLoadGameScene(SDL_Renderer* renderer, SDL_Texture* bgMenu, SDL_Texture* texSearchBar, SDL_Texture* texPanel, UIButton btnLoad, UIButton btnRename, UIButton btnDelete, UITextBox* searchBox, std::vector<std::string> fileList, int selectedFileIndex, int scrollOffset, TTF_Font* font60, SDL_Texture* texClose, bool hClose) {

    // 1. Vẽ Nền và UI tĩnh theo tọa độ của Kin
    DrawTexture(bgMenu, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DrawTexture(texSearchBar, renderer, 194, 24, 1588, 268);
    DrawTexture(texPanel, renderer, 77, 292, 1822, 753);

    // Vẽ 3 nút bấm
    DrawUIButton(btnLoad, renderer);
    DrawUIButton(btnRename, renderer);
    DrawUIButton(btnDelete, renderer);

    // Bảng màu
    SDL_Color colorWhite = { 255, 255, 255, 255 };
    SDL_Color colorBlack = { 0, 0, 0, 255 };
    SDL_Color colorHighlight = { 200, 0, 0, 255 }; // Đỏ nếu file được chọn

    // 2. VẼ THANH SEARCH CÓ CON TRỎ NHẤP NHÁY
    std::string searchText = searchBox->textContent;
    // Nếu ô search đang kích hoạt, và mỗi nửa giây trôi qua -> thêm dấu '|'
    if (searchBox->isActive && (SDL_GetTicks() / 500) % 2 == 0) {
        searchText += "|";
    }
    // In chữ thanh Search (x: 287, y: 123)
    DrawTextLeft(renderer, font60, searchText, searchBox->coordinateX, searchBox->coordinateY, colorWhite);

    // 3. VẼ DANH SÁCH FILE VỚI MẶT NẠ CẮT (SCROLLING)
    // Tạo vùng an toàn: (x: 100, y: 321, w: 1104, h: 701)
    SDL_Rect clipArea = { 100, 321, 1104, 701 };
    SDL_RenderSetClipRect(renderer, &clipArea); // BẬT MẶT NẠ

    // In từng file trong danh sách
    for (int i = 0; i < fileList.size(); i++) {
        // Tọa độ Y bị dịch chuyển bởi scrollOffset (cuộn chuột)
        int drawY = 321 + (i * 80) - scrollOffset; // 80 là khoảng cách mỗi dòng

        // Chỉ vẽ những file nào nằm trong tầm nhìn để tiết kiệm RAM
        if (drawY > 200 && drawY < 1050) {
            SDL_Color textColor = (i == selectedFileIndex) ? colorHighlight : colorBlack;
            DrawTextLeft(renderer, font60, fileList[i], 120, drawY, textColor);
        }
    }

    SDL_RenderSetClipRect(renderer, NULL); // TẮT MẶT NẠ (Rất quan trọng, để vẽ các thứ khác bình thường)

    // VẼ NÚT CLOSE (X) CHO LOAD GAME
    // ==========================================
    SDL_Rect rectClose = { 1813, 10, 89, 89 };

    if (hClose) {
        // Hover: Phóng to và ửng đỏ
        rectClose.x -= 5; rectClose.y -= 5;
        rectClose.w += 10; rectClose.h += 10;
        SDL_SetTextureColorMod(texClose, 255, 200, 200);
    }
    else {
        SDL_SetTextureColorMod(texClose, 255, 255, 255);
    }

    SDL_RenderCopy(renderer, texClose, NULL, &rectClose);
}

int HandleLoadGameClick(int mouseX, int mouseY, UIButton btnLoad, UIButton btnRename, UIButton btnDelete, UITextBox* searchBox, std::vector<std::string> fileList, int* selectedFileIndex, int scrollOffset, int currentState) {
    int newState = currentState;

    if (mouseX >= 1813 && mouseX <= 1813 + 89 && mouseY >= 10 && mouseY <= 10 + 89) {
        return STATE_MENU; // Quay xe về Menu ngay và luôn
    }

    // Kiểm tra click vào thanh Search (Theo tọa độ Kin cung cấp)
    if (mouseX >= searchBox->coordinateX && mouseX <= searchBox->coordinateX + searchBox->boxWidth &&
        mouseY >= searchBox->coordinateY && mouseY <= searchBox->coordinateY + searchBox->boxHeight) {
        searchBox->isActive = true;
    }
    else {
        searchBox->isActive = false;
    }

    // Kiểm tra click chọn File trong Panel
    if (mouseX >= 100 && mouseX <= 100 + 1104 && mouseY >= 321 && mouseY <= 321 + 701) {
        // Tính toán người chơi đang click vào dòng thứ mấy
        int clickedIndex = (mouseY - 321 + scrollOffset) / 80;
        if (clickedIndex >= 0 && clickedIndex < fileList.size()) {
            *selectedFileIndex = clickedIndex;
        }
    }

    return newState;
}