#include <SdCard.hpp>

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
    //Serial.print("$");
    //Serial.printf("pData->iWidth: %d, pData->y: %d\n", pData->iWidth, pData->y);

    uint16_t lineBuffer[pData->iWidth];
    png.getLineAsRGB565(pData, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff); // PNG_RGB565_BIG_ENDIAN // PNG_RGB565_LITTLE_ENDIAN
    uint16_t pos = pData->y * pData->iWidth;
    for (int x = 0; x < pData->iWidth; x++){
        if(pos < pngMemorySize) {
            pngImage[pos++] = lineBuffer[x];
        } else {
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

    //Serial.print("X");
    return 1;
}

// ***********************

SdCard::SdCard()
{
    isOk = false;
}

void SdCard::Setup()
{
    SD_MMC.setPins(PIN_SD_CLK, PIN_SD_CMD, PIN_SD_D0);
    if (!SD_MMC.begin("/sdcard", true, true))
    {
        Serial.println("Card Mount Failed");
        return;
    }
    // Serial.println("Card Mount Successful");
    isOk = true;
}

bool SdCard::IsOk()
{
    return isOk;
}

bool SdCard::LoadFile(const char *filename, uint8_t *dataArray, uint16_t size)
{
    if (!isOk)
    {
        Serial.println("Could not load sd file, not ready");
        return false;
    }
    // Serial.print("Loading file... "); Serial.println(filename);
    if (!SD_MMC.exists(filename))
    {
        Serial.printf("File not found: %s\n", filename);
        return false;
    }
    File file = SD_MMC.open(filename, FILE_READ);
    if (!file)
    {
        Serial.printf("Failed to open file: %s\n", filename);
        return false;
    }
    int index = 0;
    while (file.available() && index < size)
    {
        dataArray[index] = file.read();
        index++;
    }
    file.close();
    return true;
}

bool SdCard::LoadPngFile(const char *filename)
{
    png.setBuffer(NULL);
    int rc = png.open(filename, cbPngOpen, cbPngClose, cbPngRead, cbPngSeek, cbPngData);
    if (rc != PNG_SUCCESS)
    {
        Serial.printf("Failed to open PNG file: %s\n", filename);
        png.close();
        return false;
    }
    pngMemorySize = png.getWidth() * png.getHeight();// * sizeof(PNG_PTR_TYPE);
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
    return true;
}
