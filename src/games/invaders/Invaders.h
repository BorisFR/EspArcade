#ifndef INVADERS_H
#define INVADERS_H

//#include "../../Arduino.h"
//#include "../../Memory.h"
/*
extern void invaders_videoram_w(int offset, int data);
extern int input_port_0_r(int offset);
extern int input_port_1_r(int offset);
extern int invaders_shift_data_r(int offset);
*/
//extern RomModule invaders_rom[6];
/*
static struct MemoryReadAddress readmem[] =
    {
        {0x0000, 0x1fff, MRA_ROM},
        {0x2000, 0x3fff, MRA_RAM},
        {0x4000, 0x57ff, MRA_ROM},
        {-1}
};

static struct MemoryWriteAddress writemem[] =
    {
        {0x0000, 0x1fff, MWA_ROM},
        {0x2000, 0x23ff, MWA_RAM},
        {0x2400, 0x3fff, invaders_videoram_w, &screenData},
        {0x4000, 0x57ff, MWA_ROM},
        {-1}
};

static struct IOReadPort readport[] =
    {
        {0x01, 0x01, input_port_0_r},
        {0x02, 0x02, input_port_1_r},
        {0x03, 0x03, invaders_shift_data_r},
        {-1}
};
*/
#endif