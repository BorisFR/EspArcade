#include "Invaders.hpp"

Invaders::Invaders()
{
    IsReady = false;
}

Invaders::~Invaders()
{
    if(pngImage != nullptr) {
        free(pngImage);
        pngImage = nullptr;
    }
    // std::cout << "~Invaders()" << std::endl;
}

void Invaders::Setup(SdCard &sdCard)
{
    /*this->sdCard = sdCard;
    if(!this->sdCard.IsOk()) {
        Serial.println("SdCard not ready");
        return;
    }
    Serial.println("SdCard OK");*/
    if (!this->I8080MemoryInitialize(RAMSIZE)) {
        Serial.println("Not enought memory for RAM");
        return;
    }
    // 8  => 3 - PNG_PIXEL_INDEXED
    // 24 => 2 - PNG_PIXEL_TRUECOLOR
    // 32 => 6 - PNG_PIXEL_TRUECOLOR_ALPHA
    if(!sdCard.LoadPngFile("/si/background24.png")) {
        return;
    }
    if(!sdCard.LoadFile("/si/invaders.rom", memory, INVADERS_ROM_SIZE)) {
        return;
    }
    cpu.Connect(this, this);
    port0 = 0x0e;
    port1 = 0x00;
    port2 = 0x00;
    state = STATE_IDLE;
    IsReady = true;
    for (uint16_t y = 0; y < INVADERS_WIDTH; y++)
    {
        for (uint16_t x = 0; x < INVADERS_HEIGHT; x++)
        {
            uint32_t pos = (y) + (INVADERS_HEIGHT - x - 1) * INVADERS_WIDTH;
            screen[pos] = 0;
            screenOld[pos] = 0;
        }
    }
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

    memcpy(screenOld, screen, INVADERS_WIDTH * INVADERS_HEIGHT);
    /*for (uint16_t y = 0; y < INVADERS_WIDTH; y++)
    {
        for (uint16_t x = 0; x < INVADERS_HEIGHT; x++)
        {
            uint32_t pos = (y) + (INVADERS_HEIGHT - x - 1) * INVADERS_WIDTH;
            screenOld[pos] = screen[pos];
        }
    }*/

    uint16_t vram = 0x2400;
    for (uint16_t col = 0; col < INVADERS_WIDTH; col++)
    {
        for (uint16_t row = INVADERS_HEIGHT; row > 0; row -= 8)
        {
            uint8_t value = cpu.MemoryRead(vram);
            for (uint8_t j = 0; j < 8; j++)
            {
                uint32_t index = (col) + (row - j - 1) * INVADERS_WIDTH;
                if (value & 1 << j)
                {
                    screen[index] = 255;
                }
                else
                {
                    screen[index] = 0;
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