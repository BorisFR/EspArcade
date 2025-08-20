#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define PIN_DE 40
#define PIN_HSYNC 39
#define PIN_VSYNC 41
#define PIN_PCLK 42
#define PIN_R0 45
#define PIN_R1 48
#define PIN_R2 47
#define PIN_R3 21
#define PIN_R4 14
#define PIN_G0 5
#define PIN_G1 6
#define PIN_G2 7
#define PIN_G3 15
#define PIN_G4 16
#define PIN_G5 4
#define PIN_B0 8
#define PIN_B1 3
#define PIN_B2 46
#define PIN_B3 9
#define PIN_B4 1
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600
#define PCLK_FREQUENCY 16000000
#define HSYNC_POLARITY 1
#define VSYNC_POLARITY 1
#define HSYNC_FRONT_PORCH 40
#define HSYNC_PULSE_WIDTH 48
#define HSYNC_BACK_PORCH 128
#define VSYNC_FRONT_PORCH 13
#define VSYNC_PULSE_WIDTH 3
#define VSYNC_BACK_PORCH 45
#define SCREEN_ROTATION 1
#define SCREEN_AUTO_FLUSH true
#define SCREEN_BACK_LIGHT 10
#define SCREEN_ON LOW

class Display
{
public:
    Display();
    void Setup();
    void Test();
    void Clear();
    void Print(String text, uint16_t x, uint16_t y);
    void BeginWrite();
    void EndWrite();
    void DrawPng(uint8_t *pngImage, int16_t width, int16_t height);
    void Pixel(uint16_t x, uint16_t y, uint16_t color);
    void ClearRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

private:
    Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
        PIN_DE, PIN_VSYNC, PIN_HSYNC, PIN_PCLK,
        PIN_R0, PIN_R1, PIN_R2, PIN_R3, PIN_R4,
        PIN_G0, PIN_G1, PIN_G2, PIN_G3, PIN_G4, PIN_G5,
        PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4,
        HSYNC_POLARITY, HSYNC_FRONT_PORCH, HSYNC_PULSE_WIDTH, HSYNC_BACK_PORCH,
        VSYNC_POLARITY, VSYNC_FRONT_PORCH, VSYNC_PULSE_WIDTH, VSYNC_BACK_PORCH);
    Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
        SCREEN_WIDTH, SCREEN_HEIGHT, rgbpanel, SCREEN_ROTATION, SCREEN_AUTO_FLUSH);
};

#endif
