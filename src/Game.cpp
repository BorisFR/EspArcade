#include "Game.hpp"

Game::Game()
{
    Serial.println("Game created");
    isReady = false;
    InitCrc32Table();
}

Game::~Game() { Serial.println("Game destroyed"); }

void Game::Setup(SdCard &sdCard)
{
    Serial.println("Game setup");
    isReady = true;
}
bool Game::IsReady() { return isReady; }

void Game::Loop() {}

bool Game::InitializeMemory(uint64_t size)
{
    boardMemorySize = size;
    Serial.println("Initializing memory");
    boardMemory = (uint8_t *)malloc(boardMemorySize * sizeof(uint8_t));
    if (boardMemory == NULL)
    {
        Serial.println("Error allocating memory");
        return false;
    }
#ifdef ESP32
    Serial.println("Memory allocated: " + String(boardMemorySize) + " bytes");
#else
    Serial.println("Memory allocated: " + std::to_string(boardMemorySize) + " bytes");
#endif
    for (uint64_t i = 0; i < boardMemorySize; i++)
    {
        boardMemory[i] = 0;
    }
    return true;
}

void Game::InitCrc32Table() {
    const uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

uint32_t Game::GetCrc32(uint64_t offset, uint64_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint64_t i = 0; i < length; ++i) {
        uint8_t index = (crc ^ boardMemory[i + offset]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[index];
    }
    return crc ^ 0xFFFFFFFF;
}

bool Game::InitializeScreen(uint32_t width, uint32_t height)
{
    screenWidth = width;
    screenHeight = height;
    Serial.println("Initializing screen");
    screenData = (uint8_t *)malloc(screenWidth * screenHeight * sizeof(uint8_t));
    if (screenData == NULL)
    {
        Serial.println("Error allocating screen memory");
        return false;
    }
    for (uint16_t i = 0; i < screenWidth * screenHeight; i++)
    {
        screenData[i] = 0;
    }
    screenDataOld = (uint8_t *)malloc(screenWidth * screenHeight * sizeof(uint8_t));
    if (screenDataOld == NULL)
    {
        Serial.println("Error allocating old screen memory");
        return false;
    }
    for (uint16_t i = 0; i < screenWidth * screenHeight; i++)
    {
        screenDataOld[i] = 0;
    }
#ifdef ESP32
    Serial.println("Screen allocated: " + String(screenWidth * screenHeight * sizeof(uint8_t)) + " bytes");
#else
    Serial.println("Screen allocated: " + std::to_string(screenWidth * screenHeight * sizeof(uint8_t)) + " bytes");
#endif
    return true;
}

bool Game::Initialize(SdCard &sdCard, String folder, RomModule roms[], uint32_t number, uint32_t width, uint32_t height)
{
    Serial.println("Loading all ROMs from folder: " + folder);
    InitializeMemory(roms[0].offset);
    InitializeScreen(width, height);
    int numElements = number;
    if(numElements == 0) {
        Serial.println("No ROMs found");
        return false;
    }
#ifdef ESP32
    Serial.println("Size: " + String(roms[0].offset));
#else
    Serial.println("Size: " + std::to_string(roms[0].offset));
#endif
    for (int i = 0; i < (numElements - 1); i++)
    {
        if(roms[i].name == NULL) {
            continue;
        }
        if(roms[i].offset & ROMFLAG_DISPOSE) {

        }

        String name = String(roms[i].name);
#ifdef ESP32
        Serial.println(String(i) + " => " + name + " : " + String(roms[i].offset) + " - " + String(roms[i].length));
#else
        Serial.println(std::to_string(i) + " => " + name + " : " + std::to_string(roms[i].offset) + " - " + std::to_string(roms[i].length));
#endif
        if (!LoadRom(sdCard, folder, name, roms[i].length, roms[i].offset, roms[i].crc))
        {
            return false;
        }
    }
    return true;
}

bool Game::LoadRom(SdCard &sdCard, String folder, String filename, uint64_t size, uint64_t offset, uint32_t expectedCrc)
{
    //Serial.println("Loading ROM: " + filename);
    //Serial.println("Loading ROM: " + folder + "/" + filename);
    if (boardMemory == NULL)
    {
        Serial.println("Memory not initialized");
        return false;
    }
    if (!sdCard.LoadFile("/" + folder + "/" + filename, size, offset))
    {
        return false;
    }
    if(expectedCrc != 0 && expectedCrc != GetCrc32(offset, size))
    {
        Serial.println("Error: ROM CRC mismatch");
        //return false;
    }
    //Serial.println("Loaded ROM: " + filename + " at offset: " + String(offset) + ", size: " + String(size) + ", crc: " + String(expectedCrc));
    return true;
}
