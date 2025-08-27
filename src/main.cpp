/*
Chip is ESP32-S3 (QFN56) (revision v0.2)
Features: WiFi, BLE, Embedded PSRAM 8MB (AP_3v3)
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: 84:fc:e6:6d:5e:04

RAM:   [=         ]   7.9% (used 26016 bytes from 327680 bytes)
Flash: [=         ]   9.7% (used 634869 bytes from 6553600 bytes)
*/

#ifdef ESP32
#include "Arduino.h"
#else
#include "../lib/Arduino.h"
#endif

#include "SdCard.hpp"
SdCard sdCard = SdCard();

#include "Display.hpp"
Display display = Display();

#include "TheTouch.hpp"
TheTouch touch = TheTouch(SCREEN_WIDTH, SCREEN_HEIGHT);

#include "Game.hpp"
#include "games/invaders/Invaders.hpp"
#include "games/pacman/Pacman.hpp"
Game *game;

// #include "games/invaders/Invaders.hpp"
// Invaders invaders;
//
// #include "games/pacman/Pacman.hpp"
// Pacman pacman;

void setup()
{
  Serial.begin(115200);
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
  touch.Setup();
  display.Setup();
  sdCard.Setup();
  /*display.Print(text1, 10, 10);
  display.Print(text2, 10, 30);
  display.Print(text3, 10, 50);*/

  // game.Setup();

  /*invaders.Setup(sdCard);
  if (invaders.IsReady)
  {
    display.Print("Invaders Ok", 10, 70);
  }
  else
  {
    display.Print("Invaders KO!!!", 10, 70);
  }*/

  //  uint16_t x = 0;
  //  uint16_t y = 0;
  // #define SPACE_INVADERS_LOGO_LENGTH 480000
  // #define SPACE_INVADERS_LOGO_HEIGHT 800
  // #define SPACE_INVADERS_LOGO_WIDTH 600
  //  display.DrawPng(pngImage, SPACE_INVADERS_LOGO_WIDTH, SPACE_INVADERS_LOGO_HEIGHT);
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
  game = new Invaders();
  //game = new Pacman();
  if (!game->IsReady())
  {
    game->Setup(sdCard);
    if (game->IsReady())
    {
      display.Print("Game Ok", 10, 70);
    }
    else
    {
      display.Print("Game KO!!!", 10, 70);
    }
  }
}

//#define DELTA_X (SCREEN_HEIGHT - INVADERS_WIDTH) / 4
//#define DELTA_Y 146
// bool buttonCreditPushed = false;
// bool buttonStartPushed = false;

void loop()
{
  if (game->IsReady())
  {
    game->Loop();
  }
  display.Loop();
#ifndef ESP32
  if (display.IsKeyCredit())
  {
    game->Button(true);
  }
#endif
  touch.Loop();
  if (touch.IsTouched())
  {
    game->Button(true);
    Serial.println("Button pressed");
    // invaders.Button(true);
    //   invaders.ButtonCredit(true);
    //   buttonCreditPushed = true;
    for (int i = 0; i < touch.Touches(); i++)
    {
      display.ClearRectangle(10, 730, 300, 20);
#ifdef ESP32
      String text = "Touch " + String(i) + ": " + String(touch.Points(i).x) + "/" + String(touch.Points(i).y);
      display.Print(text, 10, 730);
#endif
      // Serial.print("  size: ");
      // Serial.println(ts.points[i].size);
    }
    // ts.isTouched = false;
  }
  /*
    invaders.Loop();
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
    */
}

#ifndef ESP32
int main()
{
  setup();
  while (!display.MustExit())
  {
    loop();
  }
  delete game;
  return 0;
}
#endif