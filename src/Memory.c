#include "Memory.h"

bool hasColor;
bool hasPalette;

unsigned char *boardMemory = NULL;
uint64_t boardMemorySize = 0;
unsigned char *gfxMemory;
uint64_t gfxMemorySize;
unsigned char *colorMemory;
uint64_t colorMemorySize;
unsigned char *soundMemory;
uint64_t soundMemorySize;
unsigned char *paletteMemory;
uint64_t paletteMemorySize;
unsigned char *tileMemory;
uint64_t tileMemorySize;
unsigned char *spriteMemory;
uint64_t spriteMemorySize;

unsigned char *colorRed;
unsigned char *colorGreen;
unsigned char *colorBlue;
uint64_t colorColorSize;
unsigned char *paletteColor;
uint64_t paletteColorSize;

unsigned char *tileGfx;
uint64_t tileWidth;
uint64_t tileHeight;
uint64_t tileNumber;
unsigned char *spriteGfx;
uint64_t spriteWidth;
uint64_t spriteHeight;
uint64_t spriteNumber;

unsigned char *screenData = NULL;
unsigned char *screenDataOld = NULL;
uint32_t screenWidth = 0;
uint32_t screenHeight = 0;

void osd_clearbitmap(struct osd_bitmap *bitmap)
{
    //int i;
    //if (bitmap != scrbitmap)
    //{
    //    for (i = 0; i < bitmap->height; i++)
    //    {
    //        if (bitmap->depth == 16)
    //            memset(bitmap->line[i], 0, 2 * bitmap->width);
    //        else
    //            memset(bitmap->line[i], 0, bitmap->width);
    //    }
    //}
}

void osd_free_bitmap(struct osd_bitmap *bitmap)
{
    if (bitmap)
    {
        if (bitmap->line)
            free(bitmap->line);
        //if (bitmap->_private)
        //    free(bitmap->_private);
        free(bitmap);
    }
}

struct osd_bitmap *osd_new_bitmap(int width, int height, int depth) /* ASG 980209 */
{
    struct osd_bitmap *bitmap;
    // myport_Printf("osd_new_bitmap. depth=> %i\n", depth);
    // if (Machine->orientation & ORIENTATION_SWAP_XY)
    //{
    //     int temp;
    //     temp = width;
    //     width = height;
    //     height = temp;
    // }
    bitmap = (struct osd_bitmap *)malloc(sizeof(struct osd_bitmap));
    if (bitmap != 0)
    {
        int i, rowlen, rdwidth;
        unsigned char *bm;
        int safety;
        if (width > 32)
            safety = 8;
        else
            safety = 0; /* don't create the safety area for GfxElement bitmaps */
                        //    if (depth != 8 && depth != 16) depth = 8;
        depth = SCREEN_DEPTH;
        bitmap->depth = depth;
        bitmap->width = width;
        bitmap->height = height;
        bitmap->line = NULL;
        // bitmap->_private = NULL;
        rdwidth = (width + 7) & ~7; /* round width to a quadword */
        if (depth == 16)
            rowlen = 2 * (rdwidth + 2 * safety) * sizeof(unsigned char);
        else
            rowlen = (rdwidth + 2 * safety) * sizeof(unsigned char);
        if ((bm = (unsigned char *)malloc((height + 2 * safety) * rowlen)) == 0)
        {
            free(bitmap);
            return 0;
        }
        if ((bitmap->line = (unsigned char **)malloc(height * sizeof(unsigned char *))) == 0)
        {
            free(bm);
            free(bitmap);
            return 0;
        }
        for (i = 0; i < height; i++)
            bitmap->line[i] = &bm[(i + safety) * rowlen + safety];
        // bitmap->_private = bm;
        osd_clearbitmap(bitmap);
    }
    return bitmap;
}