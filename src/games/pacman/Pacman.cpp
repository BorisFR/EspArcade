#include "Pacman.hpp"

Pacman::Pacman()
{
    Serial.println("Pacman created");
    //isReady = false;
    //Game();
}

Pacman::~Pacman()
{
    Serial.println("Pacman destroyed");
    //~Game();
}

void Pacman::Setup(SdCard &sdCard)
{
    Serial.println("Pacman setup");
    InitializeMemory(0x4000);
    LoadRom(sdCard, "/pacman/pacman.6e", 0x1000, 0);
    LoadRom(sdCard, "/pacman/pacman.6f", 0x1000, 0x1000);
    LoadRom(sdCard, "/pacman/pacman.6h", 0x1000, 0x2000);
    LoadRom(sdCard, "/pacman/pacman.6j", 0x1000, 0x3000);
    //isReady = true;
    Game::Setup(sdCard);
}

void Pacman::Loop()
{
}

