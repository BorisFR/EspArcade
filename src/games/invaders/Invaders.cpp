#include "Invaders.hpp"

ROM_START(invaders_rom)
ROM_REGION(0x10000) /* 64k for code */
ROM_LOAD("invaders.h", 0x0000, 0x0800, 0x734f5ad8) // 9316b-0869_m739h.h1
ROM_LOAD("invaders.g", 0x0800, 0x0800, 0x6bfaca4a) // 9316b-0856_m739g.g1
ROM_LOAD("invaders.f", 0x1000, 0x0800, 0x0ccead96) // 9316b-0855_m739f.f1
ROM_LOAD("invaders.e", 0x1800, 0x0800, 0x14e538b0) // 9316b-0854_m739e.e1
ROM_END

Invaders::Invaders()
{
    Serial.println("Invaders created");
}

Invaders::~Invaders()
{
    // if(pngImage != nullptr) {
    //     free(pngImage);
    //     pngImage = nullptr;
    // }
    Serial.println("Invaders destroyed");
}

void Invaders::Setup(SdCard &sdCard)
{
    int numElements = sizeof(invaders_rom) / sizeof(invaders_rom[0]);
    Serial.println("Size: " + String(invaders_rom[0].offset));
    InitializeMemory(invaders_rom[0].offset);
    InitializeScreen(INVADERS_WIDTH, INVADERS_HEIGHT);
    //  8  => 3 - PNG_PIXEL_INDEXED
    //  24 => 2 - PNG_PIXEL_TRUECOLOR
    //  32 => 6 - PNG_PIXEL_TRUECOLOR_ALPHA
    //  if(!sdCard.LoadPngFile("/si/background24.png")) {
    //     return;
    // }
    for (int i = 1; i < (numElements - 1); i++)
    {
        String name = String(invaders_rom[i].name);
        //Serial.println(std::to_string(i) + " => " + invaders_rom[i].name + " : " + std::to_string(invaders_rom[i].offset) + " - " + std::to_string(invaders_rom[i].length));
        Serial.println(String(i) + " => " + name + " : " + String(invaders_rom[i].offset) + " - " + String(invaders_rom[i].length));
        if (!LoadRom(sdCard, INVADERS_FOLDER, name, invaders_rom[i].length, invaders_rom[i].offset, invaders_rom[i].crc))
        {
            return;
        }
    }
    //if(LoadRom(sdCard, "si", "invaders.rom", 0x2000, 0, 0))
    //{
    //    Serial.println("Invaders ROM loaded");
    //}
    //else
    //{
    //    Serial.println("Error loading Invaders ROM");
    //    return;
    //}
    //SetMemoryReadAddresss(readmem);
    //SetMemoryWriteAddresss(writemem);
    cpu.Connect(this); //, this);
    port0 = 0x0e;
    port1 = 0x00;
    port2 = 0x00;
    state = STATE_IDLE;
    cpu.Initialize();
    Game::Setup(sdCard);
}

void Invaders::Loop()
{
    switch (state)
    {
    case STATE_IDLE:
        break;
    case STATE_CREDIT_PUSHED:
        Serial.print("Credit ON");
        ButtonCredit(true);
        state = STATE_CREDIT_RELEASED;
        lastButtonPressTime = millis();
        break;
    case STATE_CREDIT_RELEASED:
        if (millis() - lastButtonPressTime > INVADERS_DELAY_BUTTON_PUSH)
        {
            ButtonCredit(false);
            Serial.println(" OFF");
            state = STATE_START_PUSHED;
            buttonDelay = INVADERS_DELAY_BETWEEN_BUTTON_PUSH;
        }
        break;
    case STATE_START_PUSHED:
        if (buttonDelay-- == 0)
        {
            Serial.print("Start ON");
            ButtonStart(true);
            state = STATE_START_RELEASED;
            lastButtonPressTime = millis();
        }
        break;
    case STATE_START_RELEASED:
        if (millis() - lastButtonPressTime > INVADERS_DELAY_BUTTON_PUSH)
        {
            ButtonStart(false);
            Serial.println(" OFF");
            state = STATE_PLAYING;
        }
        break;
    case STATE_PLAYING:
        break;
    case STATE_GAME_OVER:
        break;
    }
    cpu.EmulateCycles(16666);
    cpu.Interrupt(0xcf); // RST 1
    cpu.EmulateCycles(16666);
    cpu.Interrupt(0xd7); // RST 2
    // std::cout << "Invaders::Loop()" << std::endl;

    // Draw the pixels from the memory locations 0x2400 - 0x3fff
    // into the window screen

    memcpy(screenDataOld, screenData, INVADERS_WIDTH * INVADERS_HEIGHT);

    uint16_t vram = 0x2400;
    for (uint16_t col = 0; col < INVADERS_WIDTH; col++)
    {
        for (uint16_t row = INVADERS_HEIGHT; row > 0; row -= 8)
        {
            uint8_t value = boardMemory[vram];
            for (uint8_t j = 0; j < 8; j++)
            {
                uint32_t index = (col) + (row - j - 1) * INVADERS_WIDTH;
                if (value & 1 << j)
                {
                    screenData[index] = 255;
                }
                else
                {
                    screenData[index] = 0;
                }
                index++;
            }
            vram++;
        }
    }
};

void Invaders::ButtonCredit(bool isPressed)
{
    if (isPressed)
    {
        port1 |= 1;
    }
    else
    {
        port1 &= ~1;
    }
}
void Invaders::ButtonStart(bool isPressed)
{
    if (isPressed)
    {
        port1 |= (1 << 2); // 0x04;
    }
    else
    {
        port1 &= ~(1 << 2); // ~0x04;
    }
}

void Invaders::Button(bool isPressed)
{
    switch (state)
    {
    case STATE_IDLE:
        if (isPressed)
        {
            state = STATE_CREDIT_PUSHED;
        }
        break;
    case STATE_CREDIT_PUSHED:
        break;
    case STATE_CREDIT_RELEASED:
        break;
    case STATE_START_PUSHED:
        break;
    case STATE_START_RELEASED:
        break;
    case STATE_PLAYING:
        break;
    case STATE_GAME_OVER:
        break;
    }
}

void Invaders::invaders_videoram_w(int offset, int data)
{
    if (screenData[offset] != data)
    {
        int i, x, y;
        int col;

        screenData[offset] = data;

        y = offset / 32;
        x = 8 * (offset % 32);

        /* Calculate overlay color for this byte */
        // col = Machine->pens[WHITE];
        // if (x >= 16 && x < 72) col = Machine->pens[GREEN];
        // if (x < 16 && y > 16 && y < 134) col = Machine->pens[GREEN];
        // if (x >= 192 && x < 224) col = Machine->pens[RED];
        //
        // for (i = 0; i < 8; i++)
        //{
        //	if (!(data & 0x01))
        //		plot_pixel_8080 (x, y, Machine->pens[BLACK]);
        //	else
        //		plot_pixel_8080 (x, y, col);
        //
        //	x ++;
        //	data >>= 1;
        //}
    }
}
