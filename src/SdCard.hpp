#ifndef IMAGE_SD_HPP
#define IMAGE_SD_HPP

#include <Arduino.h>
#include "Memory.h"

#ifdef ESP32

#include <SD_MMC.h>
#define PIN_SD_CMD 11
#define PIN_SD_CLK 12
#define PIN_SD_D0 13
#include <PNGdec.h>
#define PNG_PTR_TYPE uint8_t
extern PNG_PTR_TYPE *pngImage;
extern uint32_t pngMemorySize;

#else

#define PC_PATH "D:/DEV/RayLibArcade/"

#endif

class SdCard
{
public:
    SdCard();
    ~SdCard();

    void Setup();
    bool IsOk();
    bool LoadFile(String filename, uint64_t size, uint64_t offset = 0);
    bool LoadPngFile(const char *filename);
    

private:
    bool isOk;
};

#endif