#include <Game.hpp>

Game::Game() { Serial.println("Game created"); isReady = false;}
Game::~Game() { Serial.println("Game destroyed"); }
void Game::Setup(SdCard &sdCard) { Serial.println("Game setup"); isReady = true; }
bool Game::IsReady() { return isReady; }
void Game::Loop() { }

bool Game::InitializeMemory(uint16_t size) {
    boardMemorySize = size;
    boardMemory = (uint8_t *)malloc(boardMemorySize * sizeof(uint8_t));
    if (boardMemory == NULL) {
        Serial.println("Error allocating memory");
        return false;
    }
    for (uint16_t i = 0; i < boardMemorySize; i++) {
        boardMemory[i] = 0;
    }
    return true;
}

bool Game::LoadRom(SdCard &sdCard, const char *filename, uint16_t size, uint16_t atOffset) {
    if (boardMemory == NULL) {
        Serial.println("Memory not initialized");
        return false;
    }
    if(!sdCard.LoadFile(filename, boardMemory, size, atOffset)) {
        return false;
    }
    Serial.printf("Loaded ROM: %s at offset: %d, size: %d\n", filename, atOffset, size);
    return true;
}
