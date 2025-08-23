#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../../Game.hpp"
//#include "../../cpu/z80cpp/z80.hpp"
//#include "Pacman_rom.hpp"

/*          rom       parent    machine   inp       init */
// GAME(1980, pacman, 0, pacman, pacman, 0, ROT90, "Namco", "PuckMan (Japan set 1)")
#define PACMAN_FOLDER "pacman"
#define PACMAN_WIDTH 224
#define PACMAN_HEIGHT 288

#define PACMAN_RAMSIZE 8192
#define INSTRUCTION_PER_FRAME 1

#define PACMAN_DIP_FREE 0b00000000
#define PACMAN_DIP_1C1P 0b00000001
#define PACMAN_DIP_1C2P 0b00000010
#define PACMAN_DIP_2C1P 0b00000011

#define PACMAN_DIP_LIVE1 0b00000000
#define PACMAN_DIP_LIVE2 0b00000100
#define PACMAN_DIP_LIVE3 0b00001000
#define PACMAN_DIP_LIVE5 0b00001100

#define PACMAN_DIP_B10K 0b00000000
#define PACMAN_DIP_B15K 0b00010000
#define PACMAN_DIP_B20K 0b00100000
#define PACMAN_DIP_BNONE 0b00110000

#define PACMAN_DIP_NORMAL 0b11000000
#define PACMAN_DIP_RACKTEST 0b01000000
#define PACMAN_DIP_FREEZE 0b10000000

#define PACMAN_DIP (PACMAN_DIP_NORMAL | PACMAN_DIP_B10K | PACMAN_DIP_LIVE3 | PACMAN_DIP_1C1P)

class Pacman : public Game
{
public:
    Pacman();
    ~Pacman() override;

    //using Z80 = z80<Pacman>;

    void Setup(SdCard &sdCard) override;
    void Loop() override;

    uint8_t read8(uint16_t addr);
    void write8(uint16_t addr, uint8_t val);
    uint16_t read16(uint16_t addr);
    void write16(uint16_t addr, uint16_t val);

    unsigned char RdZ80(unsigned short Addr);
    void WrZ80(unsigned short Addr, unsigned char Value);
    unsigned char InZ80(unsigned short Port);
    void OutZ80(unsigned short Port, unsigned char Value);
    //void PatchZ80(Z80 *R);

private:
    unsigned char irq_ptr = 0;

    static constexpr int clockSpeed {static_cast<int>(3.072e6)}; // 3.072 MHz
    static constexpr int cyclesPerFrame {clockSpeed / 60};
    const int frameTime {static_cast<int>(1.0 / 60.0 * 1e3)};
    int cycles {cyclesPerFrame};

    const std::uint8_t dipswitch = 0b11001001; // game settings
    std::uint8_t input0 {0b10011111}, input1 {0b11111111U}; // default cabinet mode is upright and board test is off
    bool soundEnabled {false}, flipScreen {false};
    bool interruptEnabled {false};

    std::uint8_t spritePos[0x10] {};

    // input constants
    static constexpr std::uint8_t up {0b00000001U};
    static constexpr std::uint8_t left {0b00000010U};
    static constexpr std::uint8_t right {0b00000100U};
    static constexpr std::uint8_t down {0b00001000U};
    static constexpr std::uint8_t rackAdvance {0b00010000U};
    static constexpr std::uint8_t test {0b00010000U};
    static constexpr std::uint8_t coin1 {0b00100000U};
    static constexpr std::uint8_t onePlayer {0b00100000U};
    static constexpr std::uint8_t coin2 {0b01000000U};
    static constexpr std::uint8_t twoPlayer {0b01000000U};
    static constexpr std::uint8_t credit {0b10000000U};

    // display constants
    static constexpr std::uint32_t black {0xFF000000};
    static constexpr int screenWidth {224};
    static constexpr int screenHeight {288};
    static constexpr int scaleFactor {3};
    static constexpr int pitch {screenWidth * sizeof(std::uint32_t)};
};

#endif