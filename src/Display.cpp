#include "Display.hpp"

Display::Display()
{
    mustExit = false;
    Serial.println("Display created");
}

Display::~Display()
{
    Serial.println("Display destroyed");
}

void Display::Setup()
{
#ifdef ESP32
    if (!gfx->begin())
    {
        Serial.println("gfx->begin() failed!");
    }
#ifdef SCREEN_BACK_LIGHT
    pinMode(SCREEN_BACK_LIGHT, OUTPUT);
    digitalWrite(SCREEN_BACK_LIGHT, SCREEN_ON);
#endif
    gfx->fillScreen(RGB565_BLACK);
    gfx->setCursor(10, 10);
    gfx->setTextColor(RGB565_RED);
    gfx->println("Hello World!");
#else
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Arcade");
    SetTargetFPS(60);
#endif
}

void Display::Loop()
{
    frameCount++;
    if (frameCount > 100)
    {
        delay(100);
    }
    // DrawText("Hello", 190, 200, 20, LIGHTGRAY);
    if (millis() - lastUpdate > 1000) // Update every second
    {
        lastFrameCount = frameCount;
        frameCount = 0;
        lastUpdate = millis();
#ifdef ESP32
        Serial.print(" " + String(lastFrameCount));
#else
        Serial.print(" " + std::to_string(lastFrameCount));
#endif
    }
#ifdef ESP32

    BeginWrite();
    uint32_t index = 0;
    for (uint16_t y = 0; y < screenHeight; y++)
    {
        for (uint16_t x = 0; x < screenWidth; x++)
        {
            if (screenData[index] != screenDataOld[index])
            {
                uint16_t posX = (x * 2) + DELTA_X;
                uint16_t posY = (y * 2) + DELTA_Y;
                if (screenData[index] > 0)
                {
                    uint16_t color = WHITE;
                    if (y >= 32 && y <= 63)
                    {
                        color = RED; // RGB565(255, 96, 0); //0xFF6000;
                    }
                    else if (y >= 184 && y <= 238)
                    {
                        color = GREEN; // RGB565(27, 190, 129); // 0x1BBE81;
                    }
                    else if (y >= 240 && x >= 25 && x <= 135)
                    {
                        color = GREEN; // RGB565(27, 190, 129); //0x1BBE81;
                    }
                    Pixel(posX, posY, color);
                    Pixel(posX + 1, posY, color);
                    Pixel(posX, posY + 1, color);
                    Pixel(posX + 1, posY + 1, color);
                }
                else
                {
                    /*uint32_t index = posX + (posY * SPACE_INVADERS_LOGO_WIDTH);
                    Pixel(posX, posY, pngImage[index]);
                    Pixel(posX + 1, posY, pngImage[index + 1]);
                    index += SPACE_INVADERS_LOGO_WIDTH;
                    Pixel(posX, posY + 1, pngImage[index]);
                    Pixel(posX + 1, posY + 1, pngImage[index + 1]);*/
                    Pixel(posX, posY, BLACK);
                    Pixel(posX + 1, posY, BLACK);
                    Pixel(posX, posY + 1, BLACK);
                    Pixel(posX + 1, posY + 1, BLACK);
                }
            }
            index++;
        }
    }
    EndWrite();

#else
    if (WindowShouldClose())
    {
        mustExit = true;
        CloseWindow();
        return;
    }
    if (IsKeyPressed(KEY_C))
    {
        keyCredit = true;
    }
    else
    {
        keyCredit = false;
    }
    if(screenHeight+screenWidth == 0) return;
    BeginDrawing();
    ClearBackground(BLACK);
    for (uint16_t y = 0; y < screenHeight; y++)
    {
        for (uint16_t x = 0; x < screenWidth; x++)
        {
            uint16_t posX = (x * 2) + DELTA_X;
            uint16_t posY = (y * 2) + DELTA_Y;
            uint32_t index = x + y * screenWidth;
            Color color = (screenData[index] == 0) ? BLACK : WHITE;
            DrawPixel(posX, posY, color);
            DrawPixel(posX + 1, posY, color);
            DrawPixel(posX, posY + 1, color);
            DrawPixel(posX + 1, posY + 1, color);
        }
    }
    ClearRectangle(10, 70, 30, 20);
    Print(std::to_string(lastFrameCount), 10, 70);
    EndDrawing();
#endif
}

bool Display::MustExit()
{
    return mustExit;
}

void Display::ClearRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
#ifdef ESP32
    gfx->fillRect(x, y, width, height, RGB565_BLACK);
#else
    DrawRectangle(x, y, width, height, GRAY);
#endif
}

void Display::Print(String text, uint32_t x, uint32_t y)
{
#ifdef ESP32
    gfx->setCursor(x, y);
    gfx->setTextSize(2);
    gfx->setTextColor(RGB565_WHITE);
    gfx->println(text);
#else
    DrawText(text.c_str(), x, y, 20, WHITE);
#endif
}

void Display::Clear()
{
#ifdef ESP32
    gfx->fillScreen(RGB565_BLACK);
#endif
}

void Display::BeginWrite()
{
#ifdef ESP32
    gfx->startWrite();
#endif
}

void Display::EndWrite()
{
#ifdef ESP32
    gfx->endWrite();
#endif
}

void Display::DrawPng(uint8_t *pngImage, int16_t width, int16_t height)
{
#ifdef ESP32
    // gfx->draw24bitRGBBitmap(0, 0, pngImage, width, height);
    gfx->draw16bitRGBBitmap(0, 0, reinterpret_cast<uint16_t *>(pngImage), width, height);
#endif
}

void Display::Pixel(uint16_t x, uint16_t y, uint16_t color)
{
#ifdef ESP32
    gfx->writePixel(x, y, color);
    // gfx->drawPixel(x, y, color);
#endif
}
