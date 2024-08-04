#include "chip8.h"
#include <fstream>
#include <iostream>

Chip8::Chip8() {
    initialize();
}

void Chip8::initialize() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    for (auto& mem : memory) mem = 0;
    for (auto& reg : V) reg = 0;
    for (auto& gfx : gfx) gfx = 0;
    for (auto& key : key) key = 0;
    for (auto& stack : stack) stack = 0;

    // chip8 fontset
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; ++i) {
        memory[i] = fontset[i];
    }
}

//laedt die ROM
bool Chip8::loadROM(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    std::streampos size = file.tellg();
    char* buffer = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for (int i = 0; i < size; ++i) {
        memory[0x200 + i] = buffer[i];
    }

    delete[] buffer;
    return true;
}

void Chip8::emulateCycle() {
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode and execute opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x000: //Nop
                    pc += 2;
                    break;
                case 0x00E0: //display Clearen
                    std::fill(gfx.begin(), gfx.end(), 0);
                    drawFlag = true;
                    pc += 2;
                    break;
                case 0x00EE: //return subroutine
                    --sp;
                    pc = stack[sp];
                    pc += 2;
                    break;
                default:
                    std::cout << "Unbekannter opcode " << opcode << std::endl; 
                    pc += 2;
                    break;
            }
            break;
        case 0x1000: //Springen
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: //funktion betreten
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: //Skip if VX == NN
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x4000: //skip if VX != NN
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x5000: //Skip if VX == VY
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x6000: //VX = NN zuweisen
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000: //VX += NN addieren
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: //VX = VY kopiere in anderes Register
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0001: //Bitwise Operation ODER
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0002: //Bitwise Operation AND
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0003: //Bitwise Operation XOR
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0004: //VX += VY
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
                        V[0xF] = 1; 
                    } else {
                        V[0xF] = 0;
                    }
                    pc += 2;
                    break;
                case 0x0005: //VX -= VY
                    if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) {
                        V[0xF] = 0;
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0006: //Bitshift rechts
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                case 0x0007: //VX = VY - VX
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
                        V[0xF] = 0; 
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x000E: //Bitshift left 
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;
            }
            break;
        case 0x9000: //skip if vx != vy
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0xA000: //I = NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000: //Springe zu V0 + NNN
            pc = (opcode & 0x0FFF) + V[0];
            break;
        case 0xC000: //VX = random() & NN
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
            break;
        case 0xD000: { //Sprite zeichnen
            uint8_t Vx = V[(opcode & 0x0F00) >> 8];
            uint8_t Vy = V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;
            uint8_t pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        if (gfx[(Vx + xline + ((Vy + yline) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[Vx + xline + ((Vy + yline) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E: //Skip wenn eine Taste gedrückt wird
                    if (key[V[(opcode & 0x0F00) >> 8]] != 0) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                case 0x00A1: //Skip wenn eine Taste nciht gedrückt wird
                    if (key[V[(opcode & 0x0F00) >> 8]] == 0) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007: //VX = DT
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;
                case 0x000A: //Warten bis eine Taste gedrückt wird
                    for (int i = 0; i < 16; ++i) {
                        if (key[i] != 0) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            pc += 2;
                            break;
                        }
                    }
                    break;
                case 0x0015: //DT = VX
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0018: //ST = VX
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x001E: //I += VX
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0029: //setze I zu Fontadresse 
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;
                case 0x0033: //I = BinaryCodedDecimal von VX
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
                    pc += 2;
                    break;
                case 0x0055: //Speichere V0 bis VX in Register
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        memory[I + i] = V[i];
                    }
                    pc += 2;
                    break;
                case 0x0065: //Lade Register I in Register V0 bis VX
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        V[i] = memory[I + i];
                    }
                    pc += 2;
                    break;
            }
            break;
        default:
            std::cout << "Unbekannter opcode " << opcode << std::endl; 
            pc += 2;
            break;
    }

    //Update timers
    if (delay_timer > 0) {
        --delay_timer;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            //Beep sound
        }
        --sound_timer;
    }
}