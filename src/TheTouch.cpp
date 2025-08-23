#include "TheTouch.hpp"

TheTouch::TheTouch(uint16_t screenWidth, uint16_t screenHeight) : TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, screenHeight, screenWidth)
{
    //ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, screenHeight, screenWidth);
}

TheTouch::~TheTouch()
{
}

void TheTouch::Setup()
{
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

}

void TheTouch::Loop()
{
    this->read();
}

bool TheTouch::IsTouched()
{
    return this->isTouched;
}

uint8_t TheTouch::Touches()
{
    return this->touches;
}
