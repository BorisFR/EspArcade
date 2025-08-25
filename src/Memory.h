#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdlib.h>

extern unsigned char *boardMemory;
extern uint64_t boardMemorySize;
extern unsigned char *gfxMemory;
extern uint64_t gfxMemorySize;
extern unsigned char *colorMemory;
extern uint64_t colorMemorySize;
extern unsigned char *soundMemory;
extern uint64_t soundMemorySize;

extern unsigned char *screenData;
extern unsigned char *screenDataOld;
extern uint32_t screenWidth;
extern uint32_t screenHeight;

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

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

#define ROM_CPU               0x10000000           /* CPU ROM */
#define ROM_GFX               0x20000000           /* Graphics ROM */
#define ROM_SOUND             0x40000000           /* Sound ROM */
#define ROM_COLOR             0x80000000           /* Color ROM */


// change are made, I add ROM_CPU/GFX/SND at the end
//#define ROM_START(name) static struct RomModule name[] = {
#define ROM_START(name) RomModule name[] = {
#define ROM_REGION(length) { 0, length, 0, ROM_CPU },
#define ROM_REGION_GFX(length) { 0, length, 0, ROM_GFX },
#define ROM_REGION_SND(length) { 0, length, 0, ROM_SOUND },
#define ROM_REGION_COLOR(length) { 0, length, 0, ROM_COLOR },
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

#endif