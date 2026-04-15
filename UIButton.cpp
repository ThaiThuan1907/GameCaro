#include "UIButton.h"
#include "TextureManager.h" // G?i b?n này sang ?? dùng hàm DrawTexture

UIButton CreateButton(int startX, int startY, int width, int height, SDL_Texture* image) {
    UIButton newButton;
    newButton.coordinateX = startX;
    newButton.coordinateY = startY;
    newButton.boxWidth = width;
    newButton.boxHeight = height;
    newButton.buttonImage = image;
    newButton.isHovered = false;

    return newButton;
}

// Quy t?c 6: B?t bu?c Comment tr??c hàm logic ph?c t?p
// Hàm ki?m tra va ch?m gi?a ?i?m (chu?t) và Hình ch? nh?t (nút b?m)
// Input: D? li?u c?a nút b?m (button), t?a ?? x c?a chu?t (mouseX), t?a ?? y c?a chu?t (mouseY)
// Output: Tr? v? true n?u chu?t n?m g?n trong nút, ng??c l?i là false
bool CheckMouseHover(UIButton button, int mouseX, int mouseY) {
    bool isInside = true;

    // Ki?m tra n?u chu?t n?m ngoài vi?n Trái, Ph?i, Trên, D??i c?a nút
    if (mouseX < button.coordinateX) {
        isInside = false;
    }
    else if (mouseX > button.coordinateX + button.boxWidth) {
        isInside = false;
    }
    else if (mouseY < button.coordinateY) {
        isInside = false;
    }
    else if (mouseY > button.coordinateY + button.boxHeight) {
        isInside = false;
    }

    return isInside;
}

void DrawUIButton(UIButton button, SDL_Renderer* renderer) {
    // Kh?i t?o các bi?n t?a ?? ?? v? (m?c ??nh là gi? nguyên)
    int renderX = button.coordinateX;
    int renderY = button.coordinateY;
    int renderWidth = button.boxWidth;
    int renderHeight = button.boxHeight;

    // Logic Scale up (Phóng to) n?u chu?t ?ang tr? vào nút
    if (button.isHovered == true) {
        // T?ng kích th??c m?i c?nh lên m?t chút (ví d? 10 pixel)
        int scaleAmount = 10;

        renderWidth = button.boxWidth + (scaleAmount * 2);
        renderHeight = button.boxHeight + (scaleAmount * 2);

        // D?ch t?a ?? X, Y lùi l?i ?? tâm c?a nút không b? l?ch khi phóng to
        renderX = button.coordinateX - scaleAmount;
        renderY = button.coordinateY - scaleAmount;
    }

    // T?n d?ng hàm c?a TextureManager ?ã vi?t ? bài tr??c ?? v?
    DrawTexture(button.buttonImage, renderer, renderX, renderY, renderWidth, renderHeight);
}