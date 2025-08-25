#ifndef GAMES_HPP
#define GAMES_HPP

#ifdef ESP32
#include "Arduino.h"
#else
#include "../lib/Arduino.h"
#endif

#include <cstdlib>
#include <cstring>
#include "SdCard.hpp"

//#include <vector>

class Game
{
public:
    Game();
    virtual ~Game();
    virtual void Setup(SdCard &sdCard);
    virtual void Loop();
    bool IsReady();

    virtual void Button(bool isPressed) {};

    //std::vector<RomModule> RomCpu;
    //std::vector<RomModule> RomGfx;
    //std::vector<RomModule> RomColor;
    //std::vector<RomModule> RomSound;

protected:
    bool isReady;
    bool InitializeMemory(uint64_t size);
    bool InitializeScreen(uint32_t width, uint32_t height);
    bool Initialize(SdCard &sdCard, String folder, RomModule roms[], uint32_t number, uint32_t width, uint32_t height);
    bool LoadRom(SdCard &sdCard, String folder, String filename, unsigned char *toMemory, uint64_t size, uint64_t offset = 0, uint32_t expectedCrc = 0);
    //void SetMemoryReadAddresss(MemoryReadAddress *readmem) { this->readMemory = readmem; }
    //void SetMemoryWriteAddresss(MemoryWriteAddress *writemem) { this->writeMemory = writemem; }

private:
    uint32_t crc32_table[256];
    void InitCrc32Table();
    uint32_t GetCrc32(uint64_t offset, uint64_t length);
    unsigned int romType;

    //MemoryReadAddress *readMemory;
    //MemoryWriteAddress *writeMemory;

};

#endif