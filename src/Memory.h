#ifndef MEMORY_H
#define MEMORY_H

#ifndef ESP32
#include "../lib/Arduino.h"
#else
#include "Arduino.h"
#endif
#include <stdint.h>
#include <stdlib.h>

extern bool hasColor;
extern bool hasPalette;


extern unsigned char *boardMemory;
extern uint64_t boardMemorySize;
extern unsigned char *gfxMemory;
extern uint64_t gfxMemorySize;
extern unsigned char *colorMemory;
extern uint64_t colorMemorySize;
extern unsigned char *soundMemory;
extern uint64_t soundMemorySize;
extern unsigned char *paletteMemory;
extern uint64_t paletteMemorySize;
extern unsigned char *tileMemory;
extern uint64_t tileMemorySize;
extern unsigned char *spriteMemory;
extern uint64_t spriteMemorySize;

extern unsigned char *colorRed;
extern unsigned char *colorGreen;
extern unsigned char *colorBlue;
extern uint64_t colorColorSize;
extern unsigned char *paletteColor;
extern uint64_t paletteColorSize;

extern unsigned char *tileGfx;
extern uint64_t tileWidth;
extern uint64_t tileHeight;
extern uint64_t tileNumber;
extern unsigned char *spriteGfx;
extern uint64_t spriteWidth;
extern uint64_t spriteHeight;
extern uint64_t spriteNumber;

extern unsigned char *screenData;
extern unsigned char *screenDataOld;
extern uint32_t screenWidth;
extern uint32_t screenHeight;

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

//extern int readbit(const unsigned char *src,int bitnum);
static int readbit(const unsigned char *src,int bitnum)
{
	return (src[bitnum / 8] >> (7 - bitnum % 8)) & 1;
}

// https://github.com/Jean-MarcHarvengt/teensyMAME/blob/master/teensyMAMEClassic4/common.h
struct RomModule
{
	const char *name;			/* name of the file to load */
	unsigned int offset;		/* offset to load it to */
	unsigned int length;		/* length of the file */
	unsigned int crc;			/* standard CRC-32 checksum */
};

// https://github.com/Jean-MarcHarvengt/teensyMAME/blob/master/teensyMAMEClassic1/common.h

#define ROMFLAG_MASK          0xf0000000           /* 4 bits worth of flags in the high nibble */
#define ROMFLAG_ALTERNATE     0x80000000           /* Alternate bytes, either even or odd */
#define ROMFLAG_DISPOSE       0x80000000           /* Dispose of this region when done */
#define ROMFLAG_IGNORE        0x40000000           /* BM: Ignored - drivers must load this region themselves */
#define ROMFLAG_WIDE          0x40000000           /* 16-bit ROM; may need byte swapping */
#define ROMFLAG_SWAP          0x20000000           /* 16-bit ROM with bytes in wrong order */

#define ROM_CPU               0x00000001           /* CPU ROM */
#define ROM_GFX               0x00000002           /* Graphics ROM */
#define ROM_SOUND             0x00000004           /* Sound ROM */
#define ROM_COLOR             0x00000008           /* Color ROM */
#define ROM_PALETTE           0x00000010           /* Palette ROM */
#define ROM_TILE              0x00000020           /* Tile ROM */
#define ROM_SPRITE            0x00000040           /* Sprite ROM */

// change are made, I add ROM_CPU/GFX/SND at the end
//#define ROM_START(name) static struct RomModule name[] = {
#define ROM_START(name) RomModule name[] = {
#define ROM_REGION(length) { 0, length, 0, ROM_CPU },
#define ROM_REGION_GFX(length) { 0, length, 0, ROM_GFX },
#define ROM_REGION_SND(length) { 0, length, 0, ROM_SOUND },
#define ROM_REGION_COLOR(length) { 0, length, 0, ROM_COLOR },
#define ROM_REGION_PALETTE(length) { 0, length, 0, ROM_PALETTE },
#define ROM_REGION_TILE(length) { 0, length, 0, ROM_TILE },
#define ROM_REGION_SPRITE(length) { 0, length, 0, ROM_SPRITE },
#define ROM_REGION_DISPOSE(length) { 0, length, 0, ROMFLAG_DISPOSE },
#define ROM_REGION_OPTIONAL(length) { 0, length, 0, ROMFLAG_IGNORE },
#define ROM_LOAD(name,offset,length,crc) { name, offset, length, crc },
#define ROM_END { 0, 0, 0, 0 } };

// from https://github.com/Jean-MarcHarvengt/teensyMAME/blob/master/teensyMAMEClassic3/memory.h

#define MAX_BANKS 8

/***************************************************************************
Note that the memory hooks are not passed the actual memory address where
the operation takes place, but the offset from the beginning of the block
they are assigned to. This makes handling of mirror addresses easier, and
makes the handlers a bit more "object oriented". If you handler needs to
read/write the main memory area, provide a "base" pointer: it will be
initialized by the main engine to point to the beginning of the memory block
assigned to the handler. You may also provided a pointer to "size": it
will be set to the length of the memory area processed by the handler.
***************************************************************************/
struct MemoryReadAddress
{
	int start,end;
	int (*handler)(int offset);   /* see special values below */
	unsigned char **base;         /* optional (see explanation above) */
	int *size;                    /* optional (see explanation above) */
};

#define MRA_NOP   0	              /* don't care, return 0 */
#define MRA_RAM   ((int(*)(int))-1)	  /* plain RAM location (return its contents) */
#define MRA_ROM   ((int(*)(int))-2)	  /* plain ROM location (return its contents) */
#define MRA_BANK1 ((int(*)(int))-10)  /* bank memory */
#define MRA_BANK2 ((int(*)(int))-11)  /* bank memory */
#define MRA_BANK3 ((int(*)(int))-12)  /* bank memory */
#define MRA_BANK4 ((int(*)(int))-13)  /* bank memory */
#define MRA_BANK5 ((int(*)(int))-14)  /* bank memory */
#define MRA_BANK6 ((int(*)(int))-15)  /* bank memory */
#define MRA_BANK7 ((int(*)(int))-16)  /* bank memory */
#define MRA_BANK8 ((int(*)(int))-17)  /* bank memory */

struct MemoryWriteAddress
{
	int start,end;
	void (*handler)(int offset,int data);	/* see special values below */
	unsigned char **base;	/* optional (see explanation above) */
	int *size;	/* optional (see explanation above) */
};

#define MWA_NOP 0	                  /* do nothing */
#define MWA_RAM ((void(*)(int,int))-1)	   /* plain RAM location (store the value) */
#define MWA_ROM ((void(*)(int,int))-2)	   /* plain ROM location (do nothing) */
/* RAM[] and ROM[] are usually the same, but they aren't if the CPU opcodes are */
/* encrypted. In such a case, opcodes are fetched from ROM[], and arguments from */
/* RAM[]. If the program dynamically creates code in RAM and executes it, it */
/* won't work unless writes to RAM affects both RAM[] and ROM[]. */
#define MWA_RAMROM ((void(*)(int,int))-3)	/* write to both the RAM[] and ROM[] array. */
#define MWA_BANK1 ((void(*)(int,int))-10)  /* bank memory */
#define MWA_BANK2 ((void(*)(int,int))-11)  /* bank memory */
#define MWA_BANK3 ((void(*)(int,int))-12)  /* bank memory */
#define MWA_BANK4 ((void(*)(int,int))-13)  /* bank memory */
#define MWA_BANK5 ((void(*)(int,int))-14)  /* bank memory */
#define MWA_BANK6 ((void(*)(int,int))-15)  /* bank memory */
#define MWA_BANK7 ((void(*)(int,int))-16)  /* bank memory */
#define MWA_BANK8 ((void(*)(int,int))-17)  /* bank memory */

/***************************************************************************
IN and OUT ports are handled like memory accesses, the hook template is the
same so you can interchange them. Of course there is no 'base' pointer for
IO ports.
***************************************************************************/
struct IOReadPort
{
	int start,end;
	int (*handler)(int offset);	/* see special values below */
};

#define IORP_NOP 0	/* don't care, return 0 */


struct IOWritePort
{
	int start,end;
	void (*handler)(int offset,int data);	/* see special values below */
};

#define IOWP_NOP 0	/* do nothing */

struct GameDriver
{
	const char *source_file;	/* set this to __FILE__ */
	const struct GameDriver *clone_of;	/* if this is a clone, point to */
										/* the main version of the game */
	const char *name;
	const char *description;
	const char *year;
	const char *manufacturer;
	const char *credits;
	int flags;	/* see defines below */
	const struct MachineDriver *drv;
	void (*driver_init)(void);	/* optional function to be called during initialization */
								/* This is called ONCE, unlike Machine->init_machine */
								/* which is called every time the game is reset. */

	const struct RomModule *rom;
	void (*rom_decode)(void);		/* used to decrypt the ROMs after loading them */
	void (*opcode_decode)(void);	/* used to decrypt the CPU opcodes in the ROMs, */
									/* if the encryption is different from the above. */
	const char **samplenames;	/* optional array of names of samples to load. */
						/* drivers can retrieve them in Machine->samples */
	const unsigned char *sound_prom;

	struct InputPort *input_ports;

	/* if they are available, provide a dump of the color proms, or even */
	/* better load them from disk like the other ROMs. */
	/* If you load them from disk, you must place them in a memory region by */
	/* itself, and use the PROM_MEMORY_REGION macro below to say in which */
	/* region they are. */
	const unsigned char *color_prom;
	const unsigned char *palette;
	const unsigned short *colortable;
	int orientation;	/* orientation of the monitor; see defines below */

	int (*hiscore_load)(void);	/* will be called every vblank until it */
						/* returns nonzero */
	void (*hiscore_save)(void);	/* will not be called if hiscore_load() hasn't yet */
						/* returned nonzero, to avoid saving an invalid table */
};

struct GfxLayout
{
	unsigned short width,height; /* width and height of chars/sprites */
	unsigned int total; /* total numer of chars/sprites in the rom */
	unsigned short planes; /* number of bitplanes */
	int planeoffset[8]; /* start of every bitplane */
	int xoffset[32]; /* coordinates of the bit corresponding to the pixel */
	int yoffset[32]; /* of the given coordinates */
	short charincrement; /* distance between two consecutive characters/sprites */
};

struct GfxDecodeInfo
{
	int memory_region;	/* memory region where the data resides (usually 1) */
						/* -1 marks the end of the array */
	int start;	/* beginning of data to decode */
	struct GfxLayout *gfxlayout;
	int color_codes_start;	/* offset in the color lookup table where color codes start */
	int total_color_codes;	/* total number of color codes */
};

struct osd_bitmap
{
	int width,height;       /* width and height of the bitmap */
	int depth;		/* bits per pixel - ASG 980209 */
	//void *_private; /* don't touch! - reserved for osdepend use */
	unsigned char **line; /* pointers to the start of each line */
};

#define SCREEN_DEPTH 16
struct osd_bitmap *osd_new_bitmap(int width,int height,int depth);	/* ASG 980209 */
#define osd_create_bitmap(w,h) osd_new_bitmap((w),(h),8)		/* ASG 980209 */
void osd_clearbitmap(struct osd_bitmap *bitmap);
void osd_free_bitmap(struct osd_bitmap *bitmap);

struct GfxElement
{
	int width,height;

	struct osd_bitmap *gfxdata;	/* graphic data */
	unsigned int total_elements;	/* total number of characters/sprites */

	int color_granularity;	/* number of colors for each color code */
							/* (for example, 4 for 2 bitplanes gfx) */
	unsigned short *colortable;	/* map color codes to screen pens */
								/* if this is 0, the function does a verbatim copy */
	int total_colors;
	unsigned int *pen_usage;	/* an array of total_elements ints. */
								/* It is a table of the pens each character uses */
								/* (bit 0 = pen 0, and so on). This is used by */
								/* drawgfgx() to do optimizations like skipping */
								/* drawing of a totally transparent characters */
};

#endif