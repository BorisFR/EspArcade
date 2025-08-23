#include "Memory.h"

unsigned char *boardMemory = NULL;
uint64_t boardMemorySize = 0;

unsigned char *screenData = NULL;
unsigned char *screenDataOld = NULL;
uint32_t screenWidth = 0;
uint32_t screenHeight = 0;