#ifndef THETOUCH_HPP
#define THETOUCH_HPP

#include <Arduino.h>
#include "TAMC_GT911.h"
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_ROTATION ROTATION_RIGHT

class TheTouch : TAMC_GT911
{
public:
    TheTouch(uint16_t screenWidth, uint16_t screenHeight);
    ~TheTouch();

    void Setup();
    void Loop();
    bool IsTouched();
    uint8_t Touches();
    TP_Point Points(uint8_t index) { return this->points[index]; }

private:
    //TAMC_GT911 ts;

};

#endif