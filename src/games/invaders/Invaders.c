#include "Invaders.h"

/*
ROM_START(invaders_rom)
ROM_REGION(0x10000) // 64k for code 
ROM_LOAD("invaders.h", 0x0000, 0x0800, 0x734f5ad8)
ROM_LOAD("invaders.g", 0x0800, 0x0800, 0x6bfaca4a)
ROM_LOAD("invaders.f", 0x1000, 0x0800, 0x0ccead96)
ROM_LOAD("invaders.e", 0x1800, 0x0800, 0x14e538b0)
ROM_END
*/

/*
void invaders_videoram_w(int offset, int data) {};
// https://github.com/Jean-MarcHarvengt/teensyMAME/blob/master/teensyMAMEClassic4/inptport.h
int input_port_0_r(int offset) { return 0; };
int input_port_1_r(int offset) { return 0; };
int invaders_shift_data_r(int offset) { return 0; };
*/