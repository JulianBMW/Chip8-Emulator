#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <array>

class Chip8 {
public:
    Chip8();
    bool loadROM(const char* filename);
    void emulateCycle();

    bool drawFlag;
    std::array<uint8_t, 64 * 32> gfx;
    std::array<uint8_t, 16> key;

private:
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, 16> V;
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t opcode;

    void initialize();
};

#endif 