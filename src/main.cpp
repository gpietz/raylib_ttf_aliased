#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct TextLine {
    std::string line;
    float lineHeight = 0;
    float lineWidth = 0;
    float lineOffset = std::numeric_limits<float>::min();

    [[nodiscard]] const char *c_str() const { return line.c_str(); }
    [[nodiscard]] bool hasOffset() const { return lineOffset != std::numeric_limits<float>::min(); }
    void resetOffset() { lineOffset = std::numeric_limits<float>::min(); }
};

typedef std::vector<TextLine> TextLineVector;

TextLineVector splitTextByNewlines(const char *text) {
    TextLineVector lines;
    std::stringstream ss(text);
    std::string line;
    if (text != nullptr && strlen(text) > 0) {
        while (std::getline(ss, line, '\n')) {
            TextLine textLine{};
            textLine.line = line;
            lines.push_back(textLine);
        }
    }
    return lines;
}

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 1200;
    const int fontSize = 84;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "raylib aliased font scroll example");
    SetTargetFPS(60);
    //ToggleFullscreen();

    // Load font from file
    Font font = LoadFontEx("assets/Trueno-wml2.otf", fontSize, nullptr, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    const char msg[] = "Lorem ipsum dolor sit amet,\n"
                       "consetetur sadipscing elitr,\n"
                       "sed diam nonumy eirmod tempor\n"
                       "invidunt ut labore et dolore\n"
                       "magna aliquyam erat, sed diam\n"
                       "voluptua. At vero eos et accusam\n"
                       "et justo duo dolores et ea rebum.\n"
                       "Stet clita kasd gubergren, no sea\n"
                       "takimata sanctus est Lorem ipsum\n"
                       "dolor sit amet. Lorem ipsum dolor\n"
                       "sit amet, consetetur sadipscing elitr,\n"
                       "sed diam nonumy eirmod tempor invidunt\n"
                       "ut labore et dolore magna aliquyam erat,\n"
                       "sed diam voluptua. At vero eos et\n"
                       "accusam et justo duo dolores et ea\n"
                       "rebum. Stet clita kasd gubergren, no\n"
                       "sea takimata sanctus est Lorem ipsum\n"
                       "dolor sit amet.\n"
                       "****\n";

    // Convert text to vector of strings
    auto textLines = splitTextByNewlines(msg);

    // Get text dimensions of each line
    for (auto &textLine: textLines) {
        auto lineTextMeasure = MeasureTextEx(font, textLine.c_str(), fontSize, 0);
        textLine.lineWidth = lineTextMeasure.x;
        textLine.lineHeight = lineTextMeasure.y;
    }

    const float moveSpeed = 1.0f;

    Vector2 screenDimension = {(float) GetScreenWidth(), (float) GetScreenHeight()};
    Vector2 textPosition = {0.0f, 0.0f};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto textLine = textLines.begin(); textLine != textLines.end(); ++textLine) {
            // Reference to previous text line
            auto previousLine = textLine != textLines.begin() ? std::prev(textLine) : textLines.end();

            // Update offset for the text lines
            if (!textLine->hasOffset()) {
                textLine->lineOffset = previousLine != textLines.end()
                        ? previousLine->lineOffset + previousLine->lineHeight
                        : screenDimension.y;
            } else if (textLine->lineOffset <= -(textLine->lineHeight)) {
                continue;
            } else {
                textLine->lineOffset -= moveSpeed;
            }

            // Calculate horizontal position for line
            textPosition.x = screenDimension.x / 2 - textLine->lineWidth / 2;
            textPosition.y = textLine->lineOffset;

            // Draw the text and increase line offset
            DrawTextEx(font, textLine->c_str(), textPosition, static_cast<float>(font.baseSize), 0, WHITE);
        }

        EndDrawing();

        // Reset scrolling after it has been completely shown
        auto lastLine = std::prev(textLines.end());
        if (lastLine->hasOffset() && lastLine->lineOffset <= -(lastLine->lineHeight)) {
            auto reset = [](auto iter) { iter->resetOffset(); };
            for (auto tl = textLines.begin(); tl != textLines.end(); ++tl) reset(tl);
        }
    }

    // Unload font
    UnloadFont(font);

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}
