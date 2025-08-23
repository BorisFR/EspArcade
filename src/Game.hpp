#ifndef GAMES_HPP
#define GAMES_HPP

#include <Arduino.h>
#include <SdCard.hpp>

class Game
{
public:
    Game();
    ~Game();
    virtual void Setup(SdCard &sdCard);
    virtual void Loop();
    bool IsReady();

protected:
    bool isReady;
    bool InitializeMemory(uint16_t size);
    bool LoadRom(SdCard &sdCard, const char *filename, uint16_t size, uint16_t atOffset = 0);
    uint16_t boardMemorySize;
    uint8_t *boardMemory;

private:
};

#endif