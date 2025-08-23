#include "Pacman.hpp"

Pacman::Pacman()
{
    Serial.println("Pacman created");
}

Pacman::~Pacman()
{
    Serial.println("Pacman destroyed");
}

void Pacman::Setup(SdCard &sdCard)
{
    Serial.println("Pacman setup");
    InitializeMemory(PACMAN_RAMSIZE);
    InitializeScreen(PACMAN_WIDTH, PACMAN_HEIGHT);
    if (!LoadRom(sdCard, PACMAN_FOLDER, "/pacman.6e", 0x1000, 0))
    {
        return;
    }
    if (!LoadRom(sdCard, PACMAN_FOLDER, "/pacman.6f", 0x1000, 0x1000))
    {
        return;
    }
    if (!LoadRom(sdCard, PACMAN_FOLDER, "/pacman.6h", 0x1000, 0x2000))
    {
        return;
    }
    if (!LoadRom(sdCard, PACMAN_FOLDER, "/pacman.6j", 0x1000, 0x3000))
    {
        return;
    }
    Game::Setup(sdCard);
}

void Pacman::Loop()
{
    //cycles = cyclesPerFrame + cpu.run(cycles);
}

uint8_t Pacman::read8(uint16_t addr)
{
    addr &= 0x7FFFU;

    if (addr < 0x4000) {
        return boardMemory[addr];
    } else if (addr < 0x5000) {
        return boardMemory[addr - 0x4000];
    } else if (addr < 0x5100) { // Memory Mapped Registers
        /**
         * Read registers not used in Pac-Man
         * 0x5004: 1 player start lamp
         * 0x5005: 2 player start lamp
         * 0x5006: Coin lockout
         * 0x5007: Coin Counter
         */

        if (addr == 0x5003) {
            return flipScreen;
        } else if (addr < 0x5003 or (0x5007 < addr and addr < 0x5040)) { // IN0 (joystick and coin slot)
            return input0;
        } else if (addr < 0x5080) { // IN1 (joystick and start buttons)
            return input1;
        } else if (addr < 0x50C0) { // Dip Switch Settings
            return dipswitch;
        }
    } else {
        //SDL_Log("error: attempt to read at %04X\n", addr);
    }
    return 0xFF;
};

void Pacman::write8(uint16_t addr, uint8_t val) {
    addr &= 0x7FFFU;

    if (addr < 0x4000) {
        //SDL_Log("error: attempt to write to rom %02X at %04X\n", val, addr);
    } else if (addr < 0x5000) {
        boardMemory[addr - 0x4000] = val;
    } else if (addr < 0x5100) { // Memory Mapped Registers
        /**
         * Write registers not used in Pac-Man:
         * 0x5002: ??? Aux board enable?
         * 0x5004: 1 player start lamp
         * 0x5005: 2 player start lamp
         * 0x5006: Coin lockout
         * 0x5007: Coin Counter
         * 0x5040-0x5060: TODO sound
         * 0x50C0-0x5100: Watchdog reset
         */

        if (addr == 0x5000) { // Interrupt enable
            interruptEnabled = val & 0b1;
        } else if (addr == 0x5001) { // Sound enable
            soundEnabled = val & 0b1;
        } else if (addr == 0x5003) { // Flip screen
            flipScreen = val & 0b1;
        }  else if (0x505F < addr and addr < 0x5070) {
            spritePos[addr - 0x5060] = val;
        }
    } else {
        //SDL_Log("error: attempt to write %02X at %04X\n", val, addr);
    }
};

uint16_t Pacman::read16(uint16_t addr)
{
    return read8(addr + 1) << 8 | read8(addr);;
};

void Pacman::write16(uint16_t addr, uint16_t val) {
    write8(addr, val & 0xFF); write8(addr + 1, val >> 8); 
};

unsigned char Pacman::RdZ80(unsigned short Addr)
{
    Addr &= 0x7fff; // a15 is unused

    if (Addr < 16384)
        return boardMemory[Addr];

    if ((Addr & 0xf000) == 0x4000)
    {
        // this includes spriteram 1
        // return emulation.Memory(Addr - 0x4000);
    }

    if ((Addr & 0xf000) == 0x5000)
    {
        // get a mask of currently pressed keys
        // unsigned char keymask = buttons_get();

        if (Addr == 0x5080) // dip switch
            return PACMAN_DIP;

        if (Addr == 0x5000)
        {
            unsigned char retval = 0xff;
            /*if (keymask & BUTTON_UP)
                retval &= ~0x01;
            if (keymask & BUTTON_LEFT)
                retval &= ~0x02;
            if (keymask & BUTTON_RIGHT)
                retval &= ~0x04;
            if (keymask & BUTTON_DOWN)
                retval &= ~0x08;
            if (keymask & BUTTON_COIN)
                retval &= ~0x20;
                */
            return retval;
        }

        if (Addr == 0x5040)
        {
            unsigned char retval = 0xff; // 0xef for service
            /*if (keymask & BUTTON_START)
                retval &= ~0x20;
                */
            return retval;
        }
    }
    return 0xff;

    //    struct MemoryReadAddress readmem[] =
    //        {
    //            {0x0000, 0x3fff, MRA_ROM},
    //            {0x4000, 0x47ff, MRA_RAM},          /* video and color RAM */
    //            {0x4c00, 0x4fff, MRA_RAM},          /* including sprite codes at 4ff0-4fff */
    //            {0x5000, 0x503f, readinputport(0)}, /* IN0 */
    //            {0x5040, 0x507f, readinputport(1)}, /* IN1 */
    //            {0x5080, 0x50bf, readinputport(2)}, /* DSW1 */
    //            {0x50c0, 0x50ff, readinputport(3)}, /* DSW2 */
    //            {0x8000, 0xbfff, MRA_ROM},          /* Ms. Pac-Man / Ponpoko only */
    //            {-1}                                /* end of table */
    //        };
};

void Pacman::WrZ80(unsigned short Addr, unsigned char Value)
{
    //    static struct MemoryWriteAddress writemem[] =
    //        {
    //            {0x0000, 0x3fff, MWA_ROM},
    //            {0x4000, 0x43ff, videoram_w, &videoram, &videoram_size},
    //            {0x4400, 0x47ff, colorram_w, &colorram},
    //            {0x4c00, 0x4fef, MWA_RAM},
    //            {0x4ff0, 0x4fff, MWA_RAM, &spriteram, &spriteram_size},
    //            {0x5000, 0x5000, interrupt_enable_w},
    //            {0x5001, 0x5001, pengo_sound_enable_w},
    //            {0x5002, 0x5002, MWA_NOP},
    //            {0x5003, 0x5003, pengo_flipscreen_w},
    //            {0x5004, 0x5005, pacman_leds_w},
    //            // 	{ 0x5006, 0x5006, pacman_coin_lockout_global_w },	this breaks many games
    //            {0x5007, 0x5007, coin_counter_w},
    //            {0x5040, 0x505f, pengo_sound_w, &pengo_soundregs},
    //            {0x5060, 0x506f, MWA_RAM, &spriteram_2},
    //            {0x50c0, 0x50c0, watchdog_reset_w},
    //            {0x8000, 0xbfff, MWA_ROM},    /* Ms. Pac-Man / Ponpoko only */
    //            {0xc000, 0xc3ff, videoram_w}, /* mirror address for video ram, */
    //            {0xc400, 0xc7ef, colorram_w}, /* used to display HIGH SCORE and CREDITS */
    //            {0xffff, 0xffff, MWA_NOP},    /* Eyes writes to this location to simplify code */
    //            {-1}                          /* end of table */
    //        };
    Addr &= 0x7fff; // a15 is unused

    if ((Addr & 0xf000) == 0x4000)
    {
        // writing 85 (U, first char of UP) to the top left corner
        // is an indication that the game has booted up
        // if (Addr == 0x4000 + 985 && Value == 85)
        //    game_started = 1;

        // emulation.Memory(Addr - 0x4000, Value);
        return;
    }

    if ((Addr & 0xff00) == 0x5000)
    {
        // 0x5060 to 0x506f writes through to ram (spriteram2)
        // if ((Addr & 0xfff0) == 0x5060)
        //    emulation.Memory(Addr - 0x4000, Value);

        if (Addr == 0x5000)
        {
            //    emulation.IrqState(0, Value & 1);
            // irq_enable[0] = Value & 1;
        }

        if ((Addr & 0xffe0) == 0x5040)
        {
            // if (soundregs[Addr - 0x5040] != Value & 0x0f)
            //     soundregs[Addr - 0x5040] = Value & 0x0f;
        }
    }
};

unsigned char Pacman::InZ80(unsigned short Port)
{
    return 0;
};

void Pacman::OutZ80(unsigned short Port, unsigned char Value)
{
    //    static struct IOWritePort writeport[] =
    //        {
    //            {0x00, 0x00, interrupt_vector_w}, /* Pac-Man only */
    //            {-1}                              /* end of table */
    //        };
    irq_ptr = Value;
};

//void Pacman::PatchZ80(Z80 *R) {};
