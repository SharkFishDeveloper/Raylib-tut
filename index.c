#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

int main()
{
    InitWindow(1800, 1200, "Simple Text Editor");
    SetTargetFPS(60);

    // === Configurable Settings ===
    int fontsize = 30;
    int letterSpacing = 4;               // Space between letters (pixels)
    int lineSpacing = fontsize + 20;     // Space between lines (pixels)

    Color textColor = WHITE;
    Color backgroundColor = BLACK;
    Color cursorColor = WHITE;

    // ==============================

    int letterCount = 0;
    int cursorIndex = 0;

    int capacity = 1024;
    char *text = malloc(capacity * sizeof(char));
    text[0] = '\0';

    int framesCounter = 0;
    bool showCursor = true;

    Font font1 = LoadFontEx("../resources/fonts/monospace/Monospace.ttf", fontsize, NULL, 0);  // Font 1
    if (font1.texture.id == 0) {
        printf("Failed to load font!\n");
    }
    while (!WindowShouldClose())
    {
        int key = GetCharPressed();
        while(key > 0){
            char letter = (char)key;
            if(key >= 32 && key <= 126){
                if(letterCount >= capacity - 1){
                    capacity *= 2;
                    text = realloc(text, capacity);
                }
                text[letterCount] = letter;
                cursorIndex++;
                text[++letterCount] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_ENTER)){
            if(letterCount >= capacity - 1){
                capacity *= 2;
                text = realloc(text, capacity);
            }
            text[letterCount++] = '\n';  // Add newline character
            cursorIndex++;
            text[letterCount] = '\0';    // Null terminate
        }

        static int backspaceCounter = 0;
        if (IsKeyDown(KEY_BACKSPACE) && letterCount > 0) {
            backspaceCounter++;

            if (backspaceCounter == 1 || backspaceCounter > 15) {
                text[--letterCount] = '\0';
                cursorIndex--;
            }
        } else {
            backspaceCounter = 0;
        }

        // Cursor blink logic
        framesCounter++;
        if(framesCounter >= 30){
            showCursor = !showCursor;
            framesCounter = 0;
        }

        BeginDrawing();
        ClearBackground(backgroundColor);

        int currentX = 20;
        int currentY = 20;
        int fixedWidth = MeasureTextEx(font1, "W", fontsize, letterSpacing).x;
        for (int i = 0; i < letterCount; i++) {
            if (text[i] == '\n') {
                currentY += lineSpacing;
                currentX = 20;
            } else {
                char temp[2] = { text[i], '\0' };
                 DrawTextEx(font1, temp, (Vector2){currentX, currentY}, fontsize, letterSpacing, textColor);
                currentX += fixedWidth + letterSpacing;
            }
        }

        if (showCursor) {
            DrawTextEx(font1,"_", (Vector2){currentX, currentY}, fontsize,letterSpacing, textColor);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
