#include "Memory.h"

unsigned char *boardMemory = NULL;
uint64_t boardMemorySize = 0;
unsigned char *gfxMemory;
uint64_t gfxMemorySize;
unsigned char *colorMemory;
uint64_t colorMemorySize;
unsigned char *soundMemory;
uint64_t soundMemorySize;
unsigned char *screenData = NULL;
unsigned char *screenDataOld = NULL;
uint32_t screenWidth = 0;
uint32_t screenHeight = 0;