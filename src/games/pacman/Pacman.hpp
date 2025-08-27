#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../../Game.hpp"

#define PACMAN_FOLDER "pacman"
#define PACMAN_WIDTH 224
#define PACMAN_HEIGHT 288

/*
#define INSTRUCTION_PER_FRAME 1

#define PACMAN_DIP_FREE 0b00000000
#define PACMAN_DIP_1C1P 0b00000001
#define PACMAN_DIP_1C2P 0b00000010
#define PACMAN_DIP_2C1P 0b00000011

#define PACMAN_DIP_LIVE1 0b00000000
#define PACMAN_DIP_LIVE2 0b00000100
#define PACMAN_DIP_LIVE3 0b00001000
#define PACMAN_DIP_LIVE5 0b00001100

#define PACMAN_DIP_B10K 0b00000000
#define PACMAN_DIP_B15K 0b00010000
#define PACMAN_DIP_B20K 0b00100000
#define PACMAN_DIP_BNONE 0b00110000

#define PACMAN_DIP_NORMAL 0b11000000
#define PACMAN_DIP_RACKTEST 0b01000000
#define PACMAN_DIP_FREEZE 0b10000000

#define PACMAN_DIP (PACMAN_DIP_NORMAL | PACMAN_DIP_B10K | PACMAN_DIP_LIVE3 | PACMAN_DIP_1C1P)
*/

class Pacman : public Game
{
public:
    Pacman();
    ~Pacman() override;

    void Setup(SdCard &sdCard) override;
    void Loop() override;

private:
    GfxLayout tileLayout = {
        8, 8,                                                     /* 8*8 characters */
        256,                                                      /* 256 characters */
        2,                                                        /* 2 bits per pixel */
        {0, 4},                                                   /* the two bitplanes for 4 pixels are packed into one byte */
        {8 * 8 + 0, 8 * 8 + 1, 8 * 8 + 2, 8 * 8 + 3, 0, 1, 2, 3}, /* bits are packed in groups of four */
        {0 * 8, 1 * 8, 2 * 8, 3 * 8, 4 * 8, 5 * 8, 6 * 8, 7 * 8},
        16 * 8 /* every char takes 16 bytes */
    };
    GfxLayout spriteLayout = {
        16, 16, /* 16*16 sprites */
        64,     /* 64 sprites */
        2,      /* 2 bits per pixel */
        {0, 4}, /* the two bitplanes for 4 pixels are packed into one byte */
        {8 * 8, 8 * 8 + 1, 8 * 8 + 2, 8 * 8 + 3, 16 * 8 + 0, 16 * 8 + 1, 16 * 8 + 2, 16 * 8 + 3, 24 * 8 + 0, 24 * 8 + 1, 24 * 8 + 2, 24 * 8 + 3, 0, 1, 2, 3},
        {0 * 8, 1 * 8, 2 * 8, 3 * 8, 4 * 8, 5 * 8, 6 * 8, 7 * 8, 32 * 8, 33 * 8, 34 * 8, 35 * 8, 36 * 8, 37 * 8, 38 * 8, 39 * 8},
        64 * 8 /* every sprite takes 64 bytes */
    };

};

#endif