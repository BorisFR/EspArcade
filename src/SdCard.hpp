#ifndef IMAGE_SD_HPP
#define IMAGE_SD_HPP

#include <Arduino.h>
#include <SD_MMC.h>
#define PIN_SD_CMD 11
#define PIN_SD_CLK 12
#define PIN_SD_D0 13
#include <PNGdec.h>
#define PNG_PTR_TYPE uint8_t

extern PNG_PTR_TYPE *pngImage;
extern uint32_t pngMemorySize;
class SdCard
{
public:
    SdCard();
    void Setup();
    bool IsOk();
    bool LoadFile(const char *filename, uint8_t *dataArray, uint16_t size);
    bool LoadPngFile(const char *filename);
    

private:
    bool isOk;
};

#endif