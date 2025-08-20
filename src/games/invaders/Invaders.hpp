#ifndef INVADERS_HPP
#define INVADERS_HPP

#pragma once

#include <Arduino.h>
#include <SdCard.hpp>

//#include "Invaders_rom.hpp"
// #include "../OneGame.hpp"
#include "../../cpu/i8080/i8080.hpp"
/*          rom       parent    machine   inp       init */
// GAME( 1978, invaders, 0,        invaders, invaders, invaders, ROT270, 	"Midway", "Space Invaders" )
/*

static struct MachineDriver machine_driver_invaders =
{
    // basic machine hardware
    {
        {
            CPU_8080,
            2000000,        // 2 Mhz?
            invaders_readmem,invaders_writemem,invaders_readport,writeport_2_4,
            invaders_interrupt,2    // two interrupts per frame
        }
    },
    60, DEFAULT_60HZ_VBLANK_DURATION,       // frames per second, vblank duration
    1,      // single CPU, no need for interleaving
    init_machine_invaders,

    // video hardware
    32*8, 32*8, { 0*8, 32*8-1, 0*8, 28*8-1 },
    0,      // no gfxdecodeinfo - bitmapped display
    256, 0,		// leave extra colors for the overlay
    init_palette,

    VIDEO_TYPE_RASTER | VIDEO_SUPPORTS_DIRTY | VIDEO_MODIFIES_PALETTE,
    0,
    invaders_vh_start,
    invaders_vh_stop,
    invaders_vh_screenrefresh,

    // sound hardware
    0, 0, 0, 0,
    {
        {
            SOUND_SAMPLES,
            &invaders_samples_interface
        },
        {
            SOUND_SN76477,
            &invaders_sn76477_interface
        }
    }
};
*/

// FULL MEMORY (0x4000) = ROM (0x2000) + RAM (0x2000)
#define INVADERS_ROM_SIZE 0x2000 //8192
#define RAMSIZE 0x4000
#define INSTRUCTION_PER_FRAME 1
#define INVADERS_WIDTH 224
#define INVADERS_HEIGHT 256
#define INVADERS_DELAY_BUTTON_PUSH 50
#define INVADERS_DELAY_BETWEEN_BUTTON_PUSH 100

enum InvadersState {
    STATE_IDLE,
    STATE_CREDIT_PUSHED,
    STATE_CREDIT_RELEASED,
    STATE_START_PUSHED,
    STATE_START_RELEASED,
    STATE_PLAYING,
    STATE_GAME_OVER
};

/// @brief Invaders game
class Invaders : public I8080IoDevices, I8080Memory
{
public:
    Invaders();
    ~Invaders();
    void Setup(SdCard &sdCard);
    void Loop();
    void Button(bool isPressed);
    void ButtonCredit(bool isPressed);
    void ButtonStart(bool isPressed);
    uint8_t screen[INVADERS_WIDTH * INVADERS_HEIGHT];
    uint8_t screenOld[INVADERS_WIDTH * INVADERS_HEIGHT];
    bool IsReady;

private:
    //SdCard sdCard;
    CPU_8080 cpu;
    InvadersState state;
    uint32_t buttonDelay = 0;
    unsigned long lastButtonPressTime = 0;
};

#endif