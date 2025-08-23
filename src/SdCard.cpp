#include "SdCard.hpp"

#ifdef ESP32

static File pngFile;
PNG_PTR_TYPE *pngImage = nullptr;
uint32_t pngMemorySize;
static PNG png;

static void *cbPngOpen(const char *filename, int32_t *size)
{
    // Serial.printf("Attempting to open %s\n", filename);
    pngFile = SD_MMC.open(filename);
    *size = pngFile.size();
    return &pngFile;
}

static void cbPngClose(void *handle)
{
    if (pngFile)
        pngFile.close();
}

static int32_t cbPngRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
    if (!pngFile)
        return 0;
    // Serial.printf("Reading %d bytes from PNG file\n", length);
    return pngFile.read(buffer, length);
}

static int32_t cbPngSeek(PNGFILE *handle, int32_t position)
{
    if (!pngFile)
        return 0;
    // Serial.printf("Seeking to position %d in PNG file\n", position);
    return pngFile.seek(position);
}

static int cbPngData(PNGDRAW *pData)
{
    // Serial.print("$");
    // Serial.printf("pData->iWidth: %d, pData->y: %d\n", pData->iWidth, pData->y);

    uint16_t lineBuffer[pData->iWidth];
    png.getLineAsRGB565(pData, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff); // PNG_RGB565_BIG_ENDIAN // PNG_RGB565_LITTLE_ENDIAN
    uint16_t pos = pData->y * pData->iWidth;
    for (int x = 0; x < pData->iWidth; x++)
    {
        if (pos < pngMemorySize)
        {
            pngImage[pos++] = lineBuffer[x];
        }
        else
        {
            Serial.printf("Warning: PNG image data exceeds allocated memory (pos: %d, memorySize: %d)\n", pos, pngMemorySize);
        }
    }

    // Serial.print("$");
    /*uint16_t usPixels[600];
    png.getLineAsRGB565(pData, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
    Serial.print("X");
    return 1;*/
    // Serial.printf("pData->pUser: %d, pData->iWidth: %d, pData->y: %d\n", pData->pUser, pData->iWidth, pData->y);
    /*
    uint8_t *pPalette = pData->pPalette;
    uint8_t *pPixels = pData->pPixels;
    uint16_t pos = pData->y * pData->iWidth;
    for (int x = 0; x < pData->iWidth; x++)
    {
        uint8_t index = pPixels[x];          // Get the palette index
        uint8_t r = pPalette[index * 3];     // Red component
        uint8_t g = pPalette[index * 3 + 1]; // Green component
        uint8_t b = pPalette[index * 3 + 2]; // Blue component
        uint16_t r16 = (r << 8) | r;         // Duplicate bits for full range
        uint16_t g16 = (g << 8) | g;
        uint16_t b16 = (b << 8) | b;
        pngImage[pos++] = ((r16 >> 3) << 11) | ((g16 >> 2) << 5) | (b16 >> 3);
    }
*/
    // PNG_BPP_TYPE *lineBuffer = (PNG_BPP_TYPE *)pData->pUser;
    // memcpy(&lineBuffer[pData->y * pData->iWidth], pData->pPixels, pData->iWidth * sizeof(PNG_BPP_TYPE));

    // Serial.print("X");
    return 1;
}

#endif

// ***********************

SdCard::SdCard()
{
    isOk = false;
}

SdCard::~SdCard()
{
}

void SdCard::Setup()
{
    #ifdef ESP32
    SD_MMC.setPins(PIN_SD_CLK, PIN_SD_CMD, PIN_SD_D0);
    if (!SD_MMC.begin("/sdcard", true, true))
    {
        Serial.println("Card Mount Failed");
        return;
    }
    // Serial.println("Card Mount Successful");
    #endif
    isOk = true;
}

bool SdCard::IsOk()
{
    return isOk;
}

bool SdCard::LoadFile(String filename, uint64_t size, uint64_t offset)
{
    Serial.println("Loading file: " + filename);
    if (!isOk)
    {
        Serial.println("SdCard not ready");
        return false;
    }
    if (boardMemory == NULL || size == 0)
    {
        Serial.println("Invalid board memory or size");
        return false;
    }
#ifdef ESP32
    // Serial.print("Loading file... "); Serial.println(filename);
    if (!SD_MMC.exists(filename.c_str()))
    {
        Serial.println("File not found: " + filename);
        return false;
    }
    File file = SD_MMC.open(filename.c_str(), FILE_READ);
    if (!file)
    {
        Serial.println("Failed to open file: " + filename);
        return false;
    }
    uint64_t index = offset;
    while (file.available() && index < size)
    {
        boardMemory[index] = file.read();
        index++;
    }
    file.close();
#else
    std::string fullPath = std::string(PC_PATH) + "roms/" + filename;
    Serial.println("Loading file... " + fullPath);
    const char *fname = fullPath.c_str();
    FILE *pFile = fopen(fname, "rb");
    if (pFile == NULL)
    {
        Serial.println("File not found: " + filename);
        return false;
    }
    fseek(pFile, 0, SEEK_END);
    uint64_t lSize = ftell(pFile);
    rewind(pFile);
    if (size != lSize)
    {
        Serial.println("File size mismatch: " + std::to_string(size) + " != " + std::to_string(lSize));
        fclose(pFile);
        return false;
    }
    unsigned char *buffer = (unsigned char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        Serial.println("Memory malloc error");
        fclose(pFile);
        return false;
    }
    size_t result = fread(buffer, 1, lSize, pFile);
    fclose(pFile);
    if (result != lSize)
    {
        Serial.println("File reading error");
        free(buffer);
        return false;
    }
    for (uint64_t i = 0; i < lSize; ++i)
    {
        boardMemory[i + offset] = buffer[i];
    }
    free(buffer);
#endif
    return true;
}

bool SdCard::LoadPngFile(const char *filename)
{
    #ifdef ESP32
    png.setBuffer(NULL);
    int rc = png.open(filename, cbPngOpen, cbPngClose, cbPngRead, cbPngSeek, cbPngData);
    if (rc != PNG_SUCCESS)
    {
        Serial.printf("Failed to open PNG file: %s\n", filename);
        png.close();
        return false;
    }
    pngMemorySize = png.getWidth() * png.getHeight(); // * sizeof(PNG_PTR_TYPE);
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d, memorySize: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType(), pngMemorySize);
    pngImage = (PNG_PTR_TYPE *)malloc(pngMemorySize);
    if (pngImage == NULL)
    {
        Serial.println("Not enough memory for loading png");
        png.close();
        return false;
    }
    // Serial.println("Malloc ok for loading png");
    // rc = png.decode(NULL, 0);
    rc = png.decode(pngImage, 0);
    if (rc != 0)
    {
        Serial.printf("PNG decode failed with error code: %d\n", rc);
        png.close();
        return false;
    }
    png.close();
    #endif
    return true;
}