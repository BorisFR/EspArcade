#include "TheTouch.hpp"

#ifdef ESP32
TheTouch::TheTouch(uint16_t screenWidth, uint16_t screenHeight) : TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, screenHeight, screenWidth)
{
    // ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, screenHeight, screenWidth);
}
#else
TheTouch::TheTouch(uint16_t screenWidth, uint16_t screenHeight)
{
}
#endif

TheTouch::~TheTouch()
{
}

void TheTouch::Setup()
{
#ifdef ESP32
    pinMode(TOUCH_RST, OUTPUT);
    digitalWrite(TOUCH_RST, LOW);
    delay(1000);
    digitalWrite(TOUCH_RST, HIGH);
    delay(1000);
    digitalWrite(TOUCH_RST, LOW);
    delay(1000);
    digitalWrite(TOUCH_RST, HIGH);
    delay(1000);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    this->begin();
    this->setRotation(TOUCH_ROTATION);
#else
#endif
}

void TheTouch::Loop()
{
#ifdef ESP32
    this->read();
#else
#endif
}

bool TheTouch::IsTouched()
{
#ifdef ESP32
    return this->isTouched;
#else
    return false;
#endif
}

uint8_t TheTouch::Touches()
{
#ifdef ESP32
    return this->touches;
#else
    return 0;
#endif
}
