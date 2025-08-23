#ifndef INVADERS_HPP
#define INVADERS_HPP

#pragma once

#include "../../Game.hpp"
#include "../../cpu/i8080/i8080.hpp"
#include "../../machines/Driver8080bw.hpp"
#include "invaders.h"

#define INVADERS_FOLDER "invaders"
#define INSTRUCTION_PER_FRAME 1
#define INVADERS_WIDTH 224
#define INVADERS_HEIGHT 256
#define INVADERS_DELAY_BUTTON_PUSH 50
#define INVADERS_DELAY_BETWEEN_BUTTON_PUSH 100

enum InvadersState
{
    STATE_IDLE,
    STATE_CREDIT_PUSHED,
    STATE_CREDIT_RELEASED,
    STATE_START_PUSHED,
    STATE_START_RELEASED,
    STATE_PLAYING,
    STATE_GAME_OVER
};


/// @brief Invaders game
class Invaders : I8080IoDevices, public Game // , I8080Memory
{
public:
    Invaders();
    ~Invaders() override;
    void Setup(SdCard &sdCard) override;
    void Loop() override;

    void Button(bool isPressed) override;
    void ButtonCredit(bool isPressed);
    void ButtonStart(bool isPressed);

private:
    CPU_8080 cpu;
    InvadersState state;
    uint32_t buttonDelay = 0;
    unsigned long lastButtonPressTime = 0;

    void invaders_videoram_w(int offset, int data);

};

#endif