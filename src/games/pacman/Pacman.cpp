#include "Pacman.hpp"

// https://github.com/Jean-MarcHarvengt/teensyMAME/blob/master/teensyMAMEClassic1/driver_pacman.c
// changes are made
ROM_START(pacman_rom)
ROM_REGION(0x10000) /* 64k for code */
ROM_LOAD("pacman.6e", 0x0000, 0x1000, 0xc1e6ab10)
ROM_LOAD("pacman.6f", 0x1000, 0x1000, 0x1a6fb2d4)
ROM_LOAD("pacman.6h", 0x2000, 0x1000, 0xbcdd1beb)
ROM_LOAD("pacman.6j", 0x3000, 0x1000, 0x817d94e3)

//ROM_REGION_GFX(0x2000) /* temporary space for graphics (disposed after conversion) */
ROM_REGION_TILE(0x1000)
ROM_LOAD("pacman.5e", 0x0000, 0x1000, 0x0c944964)
ROM_REGION_SPRITE(0x1000)
ROM_LOAD("pacman.5f", 0x0000, 0x1000, 0x958fedf9)

//ROM_REGION_COLOR(0x0120) /* color PROMs */
ROM_REGION_PALETTE(0x020)
ROM_LOAD("82s123.7f", 0x0000, 0x0020, 0x2fc650bd)
ROM_REGION_COLOR(0x0100)
ROM_LOAD("82s126.4a", 0x0000, 0x0100, 0x3eb3a8e4)

ROM_REGION_SND(0x0200) /* sound PROMs */
ROM_LOAD("82s126.1m", 0x0000, 0x0100, 0xa9cc86bf)
ROM_LOAD("82s126.3m", 0x0100, 0x0100, 0x77245b66) /* timing - not used */
ROM_END

// ***********************************************************

Pacman::Pacman()
{
    Serial.println("Pacman created");
}

Pacman::~Pacman()
{
    Serial.println("Pacman destroyed");
}

void Pacman::Setup(SdCard &sdCard)
{
    Serial.println("Pacman setup");
    if (!Initialize(sdCard, PACMAN_FOLDER, pacman_rom, SIZEOF(pacman_rom), PACMAN_WIDTH, PACMAN_HEIGHT))
    {
        return;
    }
    GfxDecodeInfo gfxdecodeinfo[] =
        {
            {ROM_TILE, 0x0000, &tileLayout, 0, 32},
            {ROM_SPRITE, 0x0000, &spriteLayout, 0, 32},
            {-1} /* end of array */
        };
    Decode(gfxdecodeinfo);
    Game::Setup(sdCard);
}

void Pacman::Loop()
{
    // cycles = cyclesPerFrame + cpu.run(cycles);
}
