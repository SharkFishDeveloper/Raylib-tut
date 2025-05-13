#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

int main()
{
    InitWindow(1800, 1200, "Simple Text Editor");
    SetTargetFPS(60);

    // === Configurable Settings ===
    int fontsize = 30;
    int letterSpacing = 4;
    int lineSpacing = fontsize + 8;
    Color textColor = WHITE;
    Color backgroundColor = BLACK;
    Color cursorColor = WHITE;
    // ==============================

    int letterCount = 0;
    int cursorIndex = 0;
    int desiredCursorX = 0;  // Used to keep horizontal cursor position for up/down arrows

    int capacity = 1024;
    char *text = malloc(capacity * sizeof(char));
    text[0] = '\0';

    int framesCounter = 0;
    bool showCursor = true;

    Font font1 = LoadFontEx("../resources/fonts/monospace/Monospace.ttf", fontsize, NULL, 0);
    if (font1.texture.id == 0) {
        printf("Failed to load font!\n");
    }

    while (!WindowShouldClose())
    {
        // Character input
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126) {
                if (letterCount >= capacity - 1) {
                    capacity *= 2;
                    text = realloc(text, capacity);
                }
                memmove(&text[cursorIndex + 1], &text[cursorIndex], letterCount - cursorIndex + 1);
                text[cursorIndex] = (char)key;
                cursorIndex++;
                letterCount++;
            }
            key = GetCharPressed();
        }

        // Enter key
        if (IsKeyPressed(KEY_ENTER)) {
            if (letterCount >= capacity - 1) {
                capacity *= 2;
                text = realloc(text, capacity);
            }
            memmove(&text[cursorIndex + 1], &text[cursorIndex], letterCount - cursorIndex + 1);
            text[cursorIndex] = '\n';
            cursorIndex++;
            letterCount++;
        }

        // Backspace key
        static int backspaceCounter = 0;
        if (IsKeyDown(KEY_BACKSPACE) && cursorIndex > 0) {
            backspaceCounter++;
            if (backspaceCounter == 1 || backspaceCounter > 15) {
                memmove(&text[cursorIndex - 1], &text[cursorIndex], letterCount - cursorIndex + 1);
                cursorIndex--;
                letterCount--;
            }
        } else {
            backspaceCounter = 0;
        }

        // Arrow Key Navigation
        if (IsKeyPressed(KEY_LEFT) && cursorIndex > 0) {
            cursorIndex--;
        }
        if (IsKeyPressed(KEY_RIGHT) && cursorIndex < letterCount) {
            cursorIndex++;
        }

        // Cursor position tracking for rendering
        int currentX = 20;
        int currentY = 20;
        int cursorX = currentX, cursorY = currentY;
        int fixedWidth = MeasureTextEx(font1, "W", fontsize, letterSpacing).x;

        int lineStartIndices[1024]; // Map of line starts
        int line = 0;
        lineStartIndices[line++] = 0;

        for (int i = 0; i <= letterCount; i++) {
            if (i == cursorIndex) {
                cursorX = currentX;
                cursorY = currentY;
            }

            if (i < letterCount) {
                if (text[i] == '\n') {
                    currentY += lineSpacing;
                    currentX = 20;
                    lineStartIndices[line++] = i + 1;
                } else {
                    currentX += fixedWidth + letterSpacing;
                }
            }
        }

        // Up Arrow: Move to previous line at same x-position
        if (IsKeyPressed(KEY_UP)) {
            int lineIndex = 0;
            for (int i = 1; i < line; i++) {
                if (lineStartIndices[i] > cursorIndex) {
                    break;
                }
                lineIndex = i;
            }

            if (lineIndex > 0) {
                int currentLineStart = lineStartIndices[lineIndex];
                int prevLineStart = lineStartIndices[lineIndex - 1];
                int col = (cursorIndex - currentLineStart);
                int targetX = (col) * (fixedWidth + letterSpacing);

                int charsOnPrevLine = currentLineStart - prevLineStart - 1;
                int moveToCol = targetX / (fixedWidth + letterSpacing);
                cursorIndex = prevLineStart + (moveToCol < charsOnPrevLine ? moveToCol : charsOnPrevLine);
            }
        }

        // Down Arrow: Move to next line at same x-position
        if (IsKeyPressed(KEY_DOWN)) {
            int lineIndex = 0;
            for (int i = 0; i < line; i++) {
                if (lineStartIndices[i] > cursorIndex) {
                    break;
                }
                lineIndex = i;
            }

            if (lineIndex < line - 1) {
                int currentLineStart = lineStartIndices[lineIndex];
                int nextLineStart = lineStartIndices[lineIndex + 1];
                int col = (cursorIndex - currentLineStart);
                int targetX = (col) * (fixedWidth + letterSpacing);

                int charsOnNextLine = (lineIndex + 2 < line ? lineStartIndices[lineIndex + 2] : letterCount) - nextLineStart - 1;
                int moveToCol = targetX / (fixedWidth + letterSpacing);
                cursorIndex = nextLineStart + (moveToCol < charsOnNextLine ? moveToCol : charsOnNextLine);
            }
        }

        // Blink logic
        framesCounter++;
        if (framesCounter >= 20) {
            showCursor = !showCursor;
            framesCounter = 0;
        }

        BeginDrawing();
        ClearBackground(backgroundColor);

        // Draw text
        currentX = 20;
        currentY = 20;
        for (int i = 0; i < letterCount; i++) {
            if (text[i] == '\n') {
                currentY += lineSpacing;
                currentX = 20;
            } else {
                char temp[2] = { text[i], '\0' };
                DrawTextEx(font1, temp, (Vector2){currentX, currentY}, fontsize, letterSpacing, textColor);
                currentX += fixedWidth + letterSpacing;
            }

            if (i + 1 == cursorIndex) {
                cursorX = currentX;
                cursorY = currentY;
            }
        }

        if (showCursor) {
            DrawTextEx(font1, "_", (Vector2){cursorX, cursorY}, fontsize, letterSpacing, cursorColor);
        }

        EndDrawing();
    }

    CloseWindow();
    free(text);
    return 0;
}
