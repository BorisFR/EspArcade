#include <Display.hpp>

Display::Display()
{
}

void Display::Setup()
{
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
}

void Display::Clear()
{
    gfx->fillScreen(RGB565_BLACK);
}

void Display::Print(String text, uint16_t x, uint16_t y)
{
    gfx->setCursor(x, y);
    gfx->setTextSize(2);
    gfx->setTextColor(RGB565_WHITE);
    gfx->println(text);
}

void Display::BeginWrite()
{
    gfx->startWrite();
}

void Display::EndWrite()
{
    gfx->endWrite();
}

void Display::DrawPng(uint8_t *pngImage, int16_t width, int16_t height) {
    //gfx->draw24bitRGBBitmap(0, 0, pngImage, width, height);
    gfx->draw16bitRGBBitmap(0, 0, reinterpret_cast<uint16_t*>(pngImage), width, height);
}

void Display::Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    gfx->writePixel(x, y, color);
    //gfx->drawPixel(x, y, color);
}

void Display::ClearRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    gfx->fillRect(x, y, width, height, RGB565_BLACK);
}

void Display::Test()
{
  gfx->setCursor(random(gfx->width()), random(gfx->height()));
  gfx->setTextColor(random(0xffff), random(0xffff));
  gfx->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
  gfx->println("Hello World!");
}
