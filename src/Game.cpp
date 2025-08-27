#include "Game.hpp"

Game::Game()
{
    Serial.println("Game created");
    isReady = false;
    hasColor = false;
    hasPalette = false;
    InitCrc32Table();
}

Game::~Game() { Serial.println("Game destroyed"); }

void Game::Setup(SdCard &sdCard)
{
    Serial.println("Game setup");
    isReady = true;
}
bool Game::IsReady() { return isReady; }

void Game::Loop() {}

bool Game::InitializeMemory(uint64_t size)
{
    boardMemorySize = size;
    Serial.println("Initializing memory");
    boardMemory = (uint8_t *)malloc(boardMemorySize * sizeof(uint8_t));
    if (boardMemory == NULL)
    {
        Serial.println("Error allocating memory");
        return false;
    }
#ifdef ESP32
    Serial.println("Memory allocated: " + String(boardMemorySize) + " bytes");
#else
    Serial.println("Memory allocated: " + std::to_string(boardMemorySize) + " bytes");
#endif
    for (uint64_t i = 0; i < boardMemorySize; i++)
    {
        boardMemory[i] = 0;
    }
    return true;
}

void Game::InitCrc32Table()
{
    const uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; ++i)
    {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; ++j)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ polynomial;
            }
            else
            {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

uint32_t Game::GetCrc32(uint64_t offset, uint64_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    for (uint64_t i = 0; i < length; ++i)
    {
        uint8_t index = (crc ^ boardMemory[i + offset]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[index];
    }
    return crc ^ 0xFFFFFFFF;
}

bool Game::InitializeScreen(uint32_t width, uint32_t height)
{
    screenWidth = width;
    screenHeight = height;
    Serial.println("Initializing screen");
    screenData = (uint8_t *)malloc(screenWidth * screenHeight * sizeof(uint8_t));
    if (screenData == NULL)
    {
        Serial.println("Error allocating screen memory");
        return false;
    }
    for (uint16_t i = 0; i < screenWidth * screenHeight; i++)
    {
        screenData[i] = 0;
    }
    screenDataOld = (uint8_t *)malloc(screenWidth * screenHeight * sizeof(uint8_t));
    if (screenDataOld == NULL)
    {
        Serial.println("Error allocating old screen memory");
        return false;
    }
    for (uint16_t i = 0; i < screenWidth * screenHeight; i++)
    {
        screenDataOld[i] = 0;
    }
#ifdef ESP32
    Serial.println("Screen allocated: " + String(screenWidth * screenHeight * sizeof(uint8_t)) + " bytes");
#else
    Serial.println("Screen allocated: " + std::to_string(screenWidth * screenHeight * sizeof(uint8_t)) + " bytes");
#endif
    return true;
}

bool Game::Initialize(SdCard &sdCard, String folder, RomModule roms[], uint32_t number, uint32_t width, uint32_t height)
{
    Serial.println("Loading all ROMs from folder: " + folder);
    // InitializeMemory(roms[0].offset);
    InitializeScreen(width, height);
    int numElements = number;
    if (numElements == 0)
    {
        Serial.println("No ROMs found");
        return false;
    }
    // #ifdef ESP32
    //     Serial.println("Size: " + String(roms[0].offset));
    // #else
    //     Serial.println("Size: " + std::to_string(roms[0].offset));
    // #endif
    for (int i = 0; i < (numElements - 1); i++)
    {
        if (roms[i].name == NULL)
        {
            switch (roms[i].crc)
            {
            case ROM_CPU:
                romType = ROM_CPU;
                boardMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to CPU memory: " + String(boardMemorySize) + " bytes");
#else
                Serial.println("Loading to CPU memory: " + std::to_string(boardMemorySize) + " bytes");
#endif
                boardMemory = (uint8_t *)malloc(boardMemorySize * sizeof(uint8_t));
                if (boardMemory == NULL)
                {
                    Serial.println("Error allocating board memory");
                    return false;
                }
                break;
            case ROM_GFX:
                romType = ROM_GFX;
                gfxMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to GFX memory: " + String(gfxMemorySize) + " bytes");
#else
                Serial.println("Loading to GFX memory: " + std::to_string(gfxMemorySize) + " bytes");
#endif
                gfxMemory = (uint8_t *)malloc(gfxMemorySize * sizeof(uint8_t));
                if (gfxMemory == NULL)
                {
                    Serial.println("Error allocating gfx memory");
                    return false;
                }
                break;
            case ROM_COLOR:
                romType = ROM_COLOR;
                colorMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to COLOR memory: " + String(colorMemorySize) + " bytes");
#else
                Serial.println("Loading to COLOR memory: " + std::to_string(colorMemorySize) + " bytes");
#endif
                colorMemory = (uint8_t *)malloc(colorMemorySize * sizeof(uint8_t));
                if (colorMemory == NULL)
                {
                    Serial.println("Error allocating color memory");
                    return false;
                }
                break;
            case ROM_TILE:
                romType = ROM_TILE;
                tileMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to TILE memory: " + String(tileMemorySize) + " bytes");
#else
                Serial.println("Loading to TILE memory: " + std::to_string(tileMemorySize) + " bytes");
#endif
                tileMemory = (uint8_t *)malloc(tileMemorySize * sizeof(uint8_t));
                if (tileMemory == NULL)
                {
                    Serial.println("Error allocating tile memory");
                    return false;
                }
                break;
            case ROM_SPRITE:
                romType = ROM_SPRITE;
                spriteMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to SPRITE memory: " + String(spriteMemorySize) + " bytes");
#else
                Serial.println("Loading to SPRITE memory: " + std::to_string(spriteMemorySize) + " bytes");
#endif
                spriteMemory = (uint8_t *)malloc(spriteMemorySize * sizeof(uint8_t));
                if (spriteMemory == NULL)
                {
                    Serial.println("Error allocating sprite memory");
                    return false;
                }
                break;
            case ROM_PALETTE:
                romType = ROM_PALETTE;
                paletteMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to PALETTE memory: " + String(paletteMemorySize) + " bytes");
#else
                Serial.println("Loading to PALETTE memory: " + std::to_string(paletteMemorySize) + " bytes");
#endif
                paletteMemory = (uint8_t *)malloc(paletteMemorySize * sizeof(uint8_t));
                if (paletteMemory == NULL)
                {
                    Serial.println("Error allocating palette memory");
                    return false;
                }
                break;
            case ROM_SOUND:
                romType = ROM_SOUND;
                soundMemorySize = roms[i].offset;
#ifdef ESP32
                Serial.println("Loading to SOUND memory: " + String(soundMemorySize) + " bytes");
#else
                Serial.println("Loading to SOUND memory: " + std::to_string(soundMemorySize) + " bytes");
#endif
                soundMemory = (uint8_t *)malloc(soundMemorySize * sizeof(uint8_t));
                if (soundMemory == NULL)
                {
                    Serial.println("Error allocating sound memory");
                    return false;
                }
                break;
            }
            continue;
        }

        String name = String(roms[i].name);
#ifdef ESP32
        Serial.println(String(i) + " => " + name + " : " + String(roms[i].offset) + " - " + String(roms[i].length));
#else
        Serial.println(std::to_string(i) + " => " + name + " : " + std::to_string(roms[i].offset) + " - " + std::to_string(roms[i].length));
#endif
        unsigned char *toMemory = NULL;
        switch (romType)
        {
        case ROM_CPU:
            if (boardMemory == NULL)
            {
                Serial.println("Board memory not initialized");
                return false;
            }
            toMemory = boardMemory;
            break;
        case ROM_GFX:
            if (gfxMemory == NULL)
            {
                Serial.println("GFX memory not initialized");
                return false;
            }
            toMemory = gfxMemory;
            break;
        case ROM_COLOR:
            if (colorMemory == NULL)
            {
                Serial.println("Color memory not initialized");
                return false;
            }
            toMemory = colorMemory;
            break;
        case ROM_PALETTE:
            if (paletteMemory == NULL)
            {
                Serial.println("Palette memory not initialized");
                return false;
            }
            toMemory = paletteMemory;
            break;
        case ROM_TILE:
            if (tileMemory == NULL)
            {
                Serial.println("Tile memory not initialized");
                return false;
            }
            toMemory = tileMemory;
            break;
        case ROM_SPRITE:
            if (spriteMemory == NULL)
            {
                Serial.println("Sound memory not initialized");
                return false;
            }
            toMemory = spriteMemory;
            break;
        case ROM_SOUND:
            if (soundMemory == NULL)
            {
                Serial.println("Sound memory not initialized");
                return false;
            }
            toMemory = soundMemory;
            break;
        default:
            Serial.println("Unknown ROM type");
            return false;
        }

        if (!LoadRom(sdCard, folder, name, toMemory, roms[i].length, roms[i].offset, roms[i].crc))
        {
            return false;
        }
        if (romType == ROM_PALETTE)
        {
            hasPalette = true;
        }
        if (romType == ROM_COLOR)
        {
            hasColor = true;
        }
    }
    if (hasColor)
    {
        Serial.println("Decoding colors...");
        if (!DecodeColor())
        {
            Serial.println("Error decoding colors");
            return false;
        }
        if (hasPalette)
        {
            Serial.println("Decoding palette...");
            if (!DecodePalette())
            {
                Serial.println("Error decoding palette");
                return false;
            }
        }
    }
    else
    {
        if (hasPalette)
        {
            Serial.println("Could not decode palette, missing colors!");
            return false;
        }
    }
    return true;
}

bool Game::LoadRom(SdCard &sdCard, String folder, String filename, unsigned char *toMemory, uint64_t size, uint64_t offset, uint32_t expectedCrc)
{
    // Serial.println("Loading ROM: " + filename);
    // Serial.println("Loading ROM: " + folder + "/" + filename);
    if (boardMemory == NULL)
    {
        Serial.println("Memory not initialized");
        return false;
    }
    if (!sdCard.LoadFile("/" + folder + "/" + filename, size, offset))
    {
        return false;
    }
    if (expectedCrc != 0 && expectedCrc != GetCrc32(offset, size))
    {
        Serial.println("Error: ROM CRC mismatch");
        // return false;
    }
    // Serial.println("Loaded ROM: " + filename + " at offset: " + String(offset) + ", size: " + String(size) + ", crc: " + String(expectedCrc));
    return true;
}

bool Game::DecodeColor()
{
    if (colorMemory == NULL)
    {
        Serial.println("Color memory not initialized");
        return false;
    }
    // Implement color decoding logic here
    colorColorSize = colorMemorySize;
    colorRed = (unsigned char *)malloc(colorColorSize);
    colorGreen = (unsigned char *)malloc(colorColorSize);
    colorBlue = (unsigned char *)malloc(colorColorSize);
    for (uint16_t c = 0; c < colorColorSize; c++)
    {
        uint8_t colorByte = colorMemory[c];
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        // Bit 0: Red (least amount); weight: 0x21 (33 dec).
        if ((colorByte & 0x01) == 0x01)
            red += 0x21;
        // Bit 1: Red; weight: 0x47 (71 dec).
        if ((colorByte & 0x02) == 0x02)
            red += 0x47;
        // Bit 2: Red (most amount); weight: 0x97 (151 dec).
        if ((colorByte & 0x04) == 0x04)
            red += 0x97;
        // Bit 3: Green (least amount); weight: 0x21 (33 dec).
        if ((colorByte & 0x08) == 0x08)
            green += 0x21;
        // Bit 4: Green; weight: 0x47 (71 dec).
        if ((colorByte & 0x10) == 0x10)
            green += 0x47;
        // Bit 5: Green (most amount); weight: 0x97 (151 dec).
        if ((colorByte & 0x20) == 0x20)
            green += 0x97;
        // Bit 6: Blue; weight: 0x51 (81 dec).
        if ((colorByte & 0x40) == 0x40)
            blue += 0x51;
        // Bit 7: Green (most amount); weight: 0xAE (174 dec).
        if ((colorByte & 0x80) == 0x80)
            blue += 0xAE;
        // colorRed[c] = (red << 16) | (green << 8) | blue;
        colorRed[c] = red;
        colorGreen[c] = green;
        colorBlue[c] = blue;
#ifdef ESP32
        Serial.println("Color " + String(c) + " / " + String(colorByte) + ": " + String(red) + ", " + String(green) + ", " + String(blue));
#else
        Serial.println("Color " + std::to_string(c) + " / " + std::to_string(colorByte) + ": " + std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue));
#endif
    }
    return true;
}

bool Game::DecodePalette()
{
    if (paletteMemory == NULL)
    {
        Serial.println("Palette memory not initialized");
        return false;
    }
    paletteColorSize = paletteMemorySize / 4;
    paletteColor = (unsigned char *)malloc(paletteColorSize * 3 * sizeof(uint8_t));
    uint16_t index = 0;
    for (uint16_t p = 0; p < paletteColorSize; p++)
    {
        uint8_t color0Number = paletteMemory[p];
        uint8_t color1Number = paletteMemory[p + 1];
        uint8_t color2Number = paletteMemory[p + 2];
        uint8_t color3Number = paletteMemory[p + 3];
        // uint8_t color0 = colorColor[color0Number];
        // uint8_t color1 = colorColor[color1Number];
        // uint8_t color2 = colorColor[color2Number];
        // uint8_t color3 = colorColor[color3Number];
        // paletteColor[index++] = colorRed[color0Number];
        // paletteColor[index++] = colorGreen[color0Number];
        // paletteColor[index++] = colorBlue[color0Number];
        // paletteColor[index++] = colorRed[color1Number];
        // paletteColor[index++] = colorGreen[color1Number];
        // paletteColor[index++] = colorBlue[color1Number];
        // paletteColor[index++] = colorRed[color2Number];
        // paletteColor[index++] = colorGreen[color2Number];
        // paletteColor[index++] = colorBlue[color2Number];
        // paletteColor[index++] = colorRed[color3Number];
        // paletteColor[index++] = colorGreen[color3Number];
        // paletteColor[index++] = colorBlue[color3Number];
    }
    return true;
}

bool Game::Decode(GfxDecodeInfo gfxdecodeinfo[])
{
    if (gfxdecodeinfo)
    {
        for (int8_t i = 0; i < MAX_GFX_ELEMENTS && gfxdecodeinfo[i].memory_region != -1; i++)
        {
#ifdef ESP32
            Serial.println("Decoding GFX " + String(i) + ": " + String(gfxdecodeinfo[i].memory_region));
            Serial.println("   Layout: " + String(gfxdecodeinfo[i].gfxlayout->width) + "x" + String(gfxdecodeinfo[i].gfxlayout->height) + " - " + String(gfxdecodeinfo[i].gfxlayout->total) + " elements, " + String(gfxdecodeinfo[i].gfxlayout->planes) + " planes");
            Serial.println("   Color Table: " + String(gfxdecodeinfo[i].color_codes_start) + " - " + String(gfxdecodeinfo[i].total_color_codes) + " colors");
#else
            Serial.println("Decoding GFX " + std::to_string(i) + ": " + std::to_string(gfxdecodeinfo[i].memory_region));
            Serial.println("   Layout: " + std::to_string(gfxdecodeinfo[i].gfxlayout->width) + "x" + std::to_string(gfxdecodeinfo[i].gfxlayout->height) + " - " + std::to_string(gfxdecodeinfo[i].gfxlayout->total) + " elements, " + std::to_string(gfxdecodeinfo[i].gfxlayout->planes) + " planes");
            Serial.println("   Color Table: " + std::to_string(gfxdecodeinfo[i].color_codes_start) + " - " + std::to_string(gfxdecodeinfo[i].total_color_codes) + " colors");
#endif
            unsigned char *fromMemory = NULL;
            unsigned char *toMemory = NULL;
            String romName = "Unknown";
            // width: gfxdecodeinfo[i].gfxlayout->width * gfxdecodeinfo[i].gfxlayout->total
            // height: gfxdecodeinfo[i].gfxlayout->height
            uint64_t currentWidth = gfxdecodeinfo[i].gfxlayout->width;
            uint64_t currentHeight = gfxdecodeinfo[i].gfxlayout->height;
            uint64_t currentTotal = gfxdecodeinfo[i].gfxlayout->total;
            switch (gfxdecodeinfo[i].memory_region)
            {
            case ROM_TILE:
                fromMemory = tileMemory;
                romName = "Tile";
                tileWidth = currentWidth;
                tileHeight = currentHeight;
                tileNumber = currentTotal;
                tileGfx = (unsigned char *)malloc(tileWidth * tileHeight * tileNumber);
                toMemory = tileGfx;
                break;
            case ROM_SPRITE:
                fromMemory = spriteMemory;
                romName = "Sprite";
                spriteWidth = currentWidth;
                spriteHeight = currentHeight;
                spriteNumber = currentTotal;
                spriteGfx = (unsigned char *)malloc(spriteWidth * spriteHeight * spriteNumber);
                toMemory = spriteGfx;
                break;
            default:
#ifdef ESP32
                Serial.println("Unknown memory region in Game::Decode: " + String(gfxdecodeinfo[i].memory_region));
#else
                Serial.println("Unknown memory region in Game::Decode: " + std::to_string(gfxdecodeinfo[i].memory_region));
#endif
                return false;
            }
            // uint8_t size = SIZEOF(gfxdecodeinfo[i].gfxlayout->planeoffset);
            short increment = gfxdecodeinfo[i].gfxlayout->charincrement / 8;
#ifdef ESP32
            Serial.println("Decoding from " + romName + " memory @ " + String(gfxdecodeinfo[i].start) + " with increment " + String(increment));
#else
            Serial.println("Decoding from " + romName + " memory @ " + std::to_string(gfxdecodeinfo[i].start) + " with increment " + std::to_string(increment));
#endif
            // https://github.com/Justin-Credible/pac-man-emulator/blob/master/emulator/Graphics/TileRenderer.cs
            for (int32_t element = 0; element < currentTotal; element++)
            {
                uint32_t tileByteStartIndex = element * increment;
                uint32_t tileByteEndIndex = tileByteStartIndex + increment;
#ifdef ESP32
                Serial.println("Decoding GFX " + String(i) + " / Decoding element " + String(element) + " at byte index " + String(tileByteStartIndex) + " to " + String(tileByteEndIndex - 1));
#else
                Serial.println("Decoding GFX " + std::to_string(i) + " / Decoding element " + std::to_string(element) + " at byte index " + std::to_string(tileByteStartIndex) + " to " + std::to_string(tileByteEndIndex - 1));
#endif
                uint32_t posX = 0, posY = 0;
                for (int64_t b = tileByteEndIndex - 1; b >= tileByteStartIndex; b--)
                {
                    uint8_t octet = fromMemory[gfxdecodeinfo[i].start + b];
                    // Bit     Usage
                    // 0       Bit 0 of pixel #1
                    // 1       Bit 0 of pixel #2
                    // 2       Bit 0 of pixel #3
                    // 3       Bit 0 of pixel #4
                    // 4       Bit 1 of pixel #1
                    // 5       Bit 1 of pixel #2
                    // 6       Bit 1 of pixel #3
                    // 7       Bit 1 of pixel #4
                    // Combine the two bits for each pixel to get the color index.
                    uint32_t pixel1ColorIndex = ((b & 0x10) >> 3) | (b & 0x01);
                    uint32_t pixel2ColorIndex = ((b & 0x20) >> 4) | ((b & 0x02) >> 1);
                    uint32_t pixel3ColorIndex = ((b & 0x40) >> 5) | ((b & 0x04) >> 2);
                    uint32_t pixel4ColorIndex = ((b & 0x80) >> 6) | ((b & 0x08) >> 3);
                    uint32_t pixel1Color = paletteColor[pixel1ColorIndex];
                    uint32_t pixel2Color = paletteColor[pixel2ColorIndex];
                    uint32_t pixel3Color = paletteColor[pixel3ColorIndex];
                    uint32_t pixel4Color = paletteColor[pixel4ColorIndex];
                    // tileGfx = (unsigned char *)malloc(tileWidth * tileHeight * tileNumber);
                    toMemory[element * currentWidth + posX + (posY + 0) * currentTotal] = pixel1Color;
                    toMemory[element * currentWidth + posX + (posY + 1) * currentTotal] = pixel2Color;
                    toMemory[element * currentWidth + posX + (posY + 2) * currentTotal] = pixel3Color;
                    toMemory[element * currentWidth + posX + (posY + 3) * currentTotal] = pixel4Color;
                    //  Place each pixel in the tile using the correct color.
                    //  Note that the bottom of the column is pixel #1 and the top is pixel #4.
                    // tile[posX, posY + 0] = new Rgba32() { R = pixel4Color.R, G = pixel4Color.G, B = pixel4Color.B, A = 255 };
                    // tile[posX, posY + 1] = new Rgba32() { R = pixel3Color.R, G = pixel3Color.G, B = pixel3Color.B, A = 255 };
                    // tile[posX, posY + 2] = new Rgba32() { R = pixel2Color.R, G = pixel2Color.G, B = pixel2Color.B, A = 255 };
                    // tile[posX, posY + 3] = new Rgba32() { R = pixel1Color.R, G = pixel1Color.G, B = pixel1Color.B, A = 255 };
                    posX++;
                    if (posX == currentWidth)
                    {
                        // Once we reach the last column, wrap around so we can render the bottom.
                        posX = 0;
                        posY = 4;
                    }
                }
            }
            fromMemory = nullptr;
            /*
    for (int i {0}; i != tiles.size(); ++i) {
        Tile& tile {tiles[i]};

        decodeStrip(tileRom, tile, i, 0, 8, 0, 1);
        decodeStrip(tileRom, tile, i, 1, 8, 0, 0);
    }
void decodeStrip(const std::uint8_t* rom, std::uint8_t* array, const int imageNum, const int stripNum, const int pitch, const int x, const int y)
{
    for (int j {0}; j != 8; ++j) {
        const std::uint8_t byte {rom[j + (stripNum * 8) + (imageNum * pitch * (pitch / 8 * 2))]};
        const int xBase {(y * pitch * 4) + ((x + 1) * 8 - 1) - j};

        array[xBase + pitch * 3] = ((byte & 0x10) >> 3U) | ((byte & 0x01U) >> 0U);
        array[xBase + pitch * 2] = ((byte & 0x20) >> 4U) | ((byte & 0x02U) >> 1U);
        array[xBase + pitch * 1] = ((byte & 0x40) >> 5U) | ((byte & 0x04U) >> 2U);
        array[xBase + pitch * 0] = ((byte & 0x80) >> 6U) | ((byte & 0x08U) >> 3U);
    }
}
            */
            /*
            // if ((gfx[i] = decodegfx(Machine->memory_region[gfxdecodeinfo[i].memory_region] + gfxdecodeinfo[i].start, gfxdecodeinfo[i].gfxlayout)) == 0)
            if ((gfx[i] = DecodeGfx(gfxMemory + gfxdecodeinfo[i].start, gfxdecodeinfo[i].gfxlayout)) == 0)
            {
                // vh_close();
                return false;
            }
            gfx[i]->colortable = colorTable[gfxdecodeinfo[i].color_codes_start];
            gfx[i]->total_colors = gfxdecodeinfo[i].total_color_codes;
            */
        }
    }

    return true;
}

void Game::DecodeChar(struct GfxElement *gfx, int num, const unsigned char *src, const struct GfxLayout *gl)
{
    int plane, x, y;
    unsigned char *dp;
    for (plane = 0; plane < gl->planes; plane++)
    {
        int offs;
        offs = num * gl->charincrement + gl->planeoffset[plane];
        for (y = 0; y < gfx->height; y++)
        {
            dp = gfx->gfxdata->line[num * gfx->height + y];
            for (x = 0; x < gfx->width; x++)
            {
                int xoffs, yoffs;
                if (plane == 0)
                    dp[x] = 0;
                else
                    dp[x] <<= 1;
                xoffs = x;
                yoffs = y;
                // if (Machine->orientation & ORIENTATION_FLIP_X)
                //	xoffs = gfx->width-1 - xoffs;
                // if (Machine->orientation & ORIENTATION_FLIP_Y)
                //	yoffs = gfx->height-1 - yoffs;
                // if (Machine->orientation & ORIENTATION_SWAP_XY)
                //{
                //	int temp;
                //	temp = xoffs;
                //	xoffs = yoffs;
                //	yoffs = temp;
                // }
                dp[x] += readbit(src, offs + gl->yoffset[yoffs] + gl->xoffset[xoffs]);
            }
        }
    }

    if (gfx->pen_usage)
    {
        /* fill the pen_usage array with info on the used pens */
        gfx->pen_usage[num] = 0;

        for (y = 0; y < gfx->height; y++)
        {
            dp = gfx->gfxdata->line[num * gfx->height + y];

            for (x = 0; x < gfx->width; x++)
            {
                gfx->pen_usage[num] |= 1 << dp[x];
            }
        }
    }
}

GfxElement *Game::DecodeGfx(const unsigned char *src, const GfxLayout *gl)
{
    struct osd_bitmap *bm;
    GfxElement *gfx = new GfxElement;
    gfx->width = gl->width;
    gfx->height = gl->height;

    // bm = (struct osd_bitmap *)malloc(sizeof(struct osd_bitmap));
    // if (bm == 0)
    //{
    //     free(gfx);
    //     return nullptr;
    // }

    if ((bm = osd_create_bitmap(gl->total * gfx->height, gfx->width)) == 0)
    {
        free(gfx);
        return nullptr;
    }

    bm->width = gl->width;
    bm->height = gl->height;
    // for (int i = 0; i < gl->height; i++)
    //     bm->line[i] = (unsigned char *)malloc(gl->width * sizeof(unsigned char));

    gfx->total_elements = gl->total;
    gfx->color_granularity = 1 << gl->planes;
    gfx->gfxdata = bm;
    gfx->pen_usage = 0;
    if (gfx->color_granularity <= 32) /* can't handle more than 32 pens */
        gfx->pen_usage = (unsigned int *)malloc(gfx->total_elements * sizeof(int));
    /* no need to check for failure, the code can work without pen_usage */

    for (uint16_t c = 0; c < gl->total; c++)
        DecodeChar(gfx, c, src, gl);
    return gfx;
}
