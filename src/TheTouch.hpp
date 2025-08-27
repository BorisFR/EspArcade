#ifndef THETOUCH_HPP
#define THETOUCH_HPP

#ifdef ESP32
#include "Arduino.h"
#else
#include "../lib/Arduino.h"
#endif

#ifdef ESP32
#ifdef GT911
#include "TAMC_GT911.h"
#endif
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_ROTATION ROTATION_RIGHT

#ifdef GT911
class TheTouch : TAMC_GT911
#else
class TheTouch
#endif
#else
class TheTouch
#endif
{
public:
    TheTouch(uint16_t screenWidth, uint16_t screenHeight);
    ~TheTouch();

    void Setup();
    void Loop();
    bool IsTouched();
    uint8_t Touches();
#ifdef ESP32
#ifdef GT911
    TP_Point Points(uint8_t index) { return this->points[index]; }
#else
    uint8_t Points(uint8_t index) { return 0; }
#endif
#else
    uint8_t Points(uint8_t index) { return 0; }
#endif

private:
};

#endif