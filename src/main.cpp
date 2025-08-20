/*
Chip is ESP32-S3 (QFN56) (revision v0.2)
Features: WiFi, BLE, Embedded PSRAM 8MB (AP_3v3)
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: 84:fc:e6:6d:5e:04

RAM:   [======    ]  58.4% (used 191312 bytes from 327680 bytes)
Flash: [=         ]  10.0% (used 655417 bytes from 6553600 bytes)
*/

#include <Arduino.h>

#include <SdCard.hpp>
SdCard sdCard = SdCard();

#include <Display.hpp>
Display display = Display();

#include "TAMC_GT911.h"
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_ROTATION ROTATION_RIGHT
TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, SCREEN_HEIGHT, SCREEN_WIDTH);

void touchBegin()
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
  ts.begin();
  ts.setRotation(TOUCH_ROTATION);
}
// #include <Game.hpp>
// Game game = Game();

#include "games/invaders/Invaders.hpp"
Invaders invaders;

// #include <galaga_logo.h>
// #include "games/invaders/space_invaders_logo.hpp"

void setup()
{
  // delay(5500);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  delay(1500);
  Serial.println("*** ESP Arcade");

  // ESP-IDF: 40405 / Free heap: 314568 / Main core: 1 / Main priority: 1
  /*Serial.print("ESP-IDF: ");
  Serial.print(ESP_IDF_VERSION, HEX);
  Serial.print(" / Free heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" / Main core: ");
  Serial.print(xPortGetCoreID());
  Serial.print(" / Main priority: ");
  Serial.println(uxTaskPriorityGet(NULL));
  String text1 = "ESP-IDF: " + String(ESP_IDF_VERSION, HEX) + " / Free heap: " + String(ESP.getFreeHeap()) + " / Main core: " + String(xPortGetCoreID()) + " / Main priority: " + String(uxTaskPriorityGet(NULL));*/
  // PSRam: 0 / SketchSize: 315920 / FreeSketchSize: 1310720
  /*Serial.print("PSRam: ");
  Serial.print(ESP.getPsramSize());
  Serial.print(" / SketchSize: ");
  Serial.print(ESP.getSketchSize());
  Serial.print(" / FreeSketchSize: ");
  Serial.println(ESP.getFreeSketchSpace());
  String text2 = "PSRam: " + String(ESP.getPsramSize()) + " / SketchSize: " + String(ESP.getSketchSize()) + " / FreeSketchSize: " + String(ESP.getFreeSketchSpace());*/
  // ESP: Model=ESP32-D0WD-V3 / Chip=3 / ChipCores=2 / SDK=v4.4.5
  /*Serial.print("ESP: Model=");
  Serial.print(ESP.getChipModel());
  Serial.print(" / Chip=");
  Serial.print(ESP.getChipRevision());
  Serial.print(" / ChipCores=");
  Serial.print(ESP.getChipCores());
  Serial.print(" / SDK=");
  Serial.println(ESP.getSdkVersion());
  String text3 = "ESP: Model=" + String(ESP.getChipModel()) + " / Chip=" + String(ESP.getChipRevision()) + " / ChipCores=" + String(ESP.getChipCores()) + " / SDK=" + String(ESP.getSdkVersion());*/
  touchBegin();
  display.Setup();
  sdCard.Setup();
  /*display.Print(text1, 10, 10);
  display.Print(text2, 10, 30);
  display.Print(text3, 10, 50);*/

  // game.Setup();
  invaders.Setup(sdCard);
  if (invaders.IsReady)
  {
    display.Print("Invaders Ok", 10, 70);
  }
  else
  {
    display.Print("Invaders KO!!!", 10, 70);
  }

  uint16_t x = 0;
  uint16_t y = 0;
#define SPACE_INVADERS_LOGO_LENGTH 480000
#define SPACE_INVADERS_LOGO_HEIGHT 800
#define SPACE_INVADERS_LOGO_WIDTH 600
  display.DrawPng(pngImage, SPACE_INVADERS_LOGO_WIDTH, SPACE_INVADERS_LOGO_HEIGHT);
  /*for (int i = 0; i < SPACE_INVADERS_LOGO_LENGTH; i++)
  {
    // display.Pixel(x, y, space_invaders_logo[i]);
    display.Pixel(x, y, pngImage[i]);
    x += 1;
    if (x >= SPACE_INVADERS_LOGO_WIDTH)
    {
      x = 0;
      y += 1;
    }
    //if (y >= SCREEN_HEIGHT)
    //{
    //  y = 0;
    //}
  }*/
}

#define DELTA_X (SCREEN_HEIGHT - INVADERS_WIDTH) / 4
#define DELTA_Y 146
unsigned long lastUpdate = 0;
uint16_t frameCount = 0;
// bool buttonCreditPushed = false;
// bool buttonStartPushed = false;

void loop()
{
  frameCount++;
  ts.read();
  if (ts.isTouched)
  {
    invaders.Button(true);
    // invaders.ButtonCredit(true);
    // buttonCreditPushed = true;
    for (int i = 0; i < ts.touches; i++)
    {
      display.ClearRectangle(10, 730, 300, 20);
      String text = "Touch " + String(i) + ": " + String(ts.points[i].x) + "/" + String(ts.points[i].y);
      display.Print(text, 10, 730);
      // Serial.print("  size: ");
      // Serial.println(ts.points[i].size);
    }
    // ts.isTouched = false;
  }
  /*  else
    {
      if (buttonStartPushed)
      {
        buttonStartPushed = false;
        invaders.ButtonStart(false);
      }
      if (buttonCreditPushed)
      {
        buttonCreditPushed = false;
        invaders.ButtonCredit(false);
        buttonStartPushed = true;
        invaders.ButtonStart(true);
      }
    }*/
  invaders.Loop();
  // display.Clear();
  display.BeginWrite();
  uint32_t index = 0;
  for (uint16_t y = 0; y < INVADERS_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < INVADERS_WIDTH; x++)
    {
      if (invaders.screen[index] != invaders.screenOld[index])
      {
        if (invaders.screen[index] > 0)
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
          display.Pixel((x * 2) + DELTA_X, (y * 2) + DELTA_Y, color);
          display.Pixel((x * 2) + 1 + DELTA_X, (y * 2) + DELTA_Y, color);
          display.Pixel((x * 2) + DELTA_X, (y * 2) + 1 + DELTA_Y, color);
          display.Pixel((x * 2) + 1 + DELTA_X, (y * 2) + 1 + DELTA_Y, color);
        }
        else
        {
          uint16_t posX = (x * 2) + DELTA_X;
          uint16_t posY = (y * 2) + DELTA_Y;
          uint32_t index = posX + (posY * SPACE_INVADERS_LOGO_WIDTH);
          display.Pixel(posX, posY, pngImage[index]);
          display.Pixel(posX + 1, posY, pngImage[index + 1]);
          index += SPACE_INVADERS_LOGO_WIDTH;
          display.Pixel(posX, posY + 1, pngImage[index]);
          display.Pixel(posX + 1, posY + 1, pngImage[index + 1]);
        }
      }
      index++;
    }
  }
  display.EndWrite();
  // display.Test();
  // delay(1000); // 1 second
  if (millis() - lastUpdate > 1000) // Update every second
  {
    display.ClearRectangle(10, 700, 30, 20);
    display.Print(String(frameCount), 10, 700);
    frameCount = 0;
    lastUpdate = millis();
  }
}
