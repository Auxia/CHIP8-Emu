#pragma once

#include <_types/_uint8_t.h>
#include <cstdint>
#include <random>

const int KEY_COUNT = 16;
const int MEMORY_SIZE = 4096;
const int REGISTER_COUNT = 16;
const int STACK_SIZE = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

// The Chip-8 has 34 instructions, each 2 bytes long.
class Chip8 {
    public:
        Chip8(); // Constructor
        
        void LoadROM(const char *filename);
        void EmulateCycle();

        uint8_t keypad[KEY_COUNT]{}; // 16 keys, 0x0 - 0xF
        uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT]{}; // 64 x 32 screen

    private:
        uint8_t memory[MEMORY_SIZE]{}; // 4K memory
        uint8_t registers[REGISTER_COUNT]{}; // 16 registers, V0 - VF
        uint16_t index{}; // Index register
        uint16_t pc{}; // Program counter
        uint16_t stack[STACK_SIZE]{}; // stack
        uint8_t sp{}; // stack pointer
        uint8_t delayTimer{}; // delay timer
        uint8_t soundTimer{}; // sound timer
        uint16_t opcode{}; // current opcode

        void Table0();
        void Table8();
        void TableE();
        void TableF();

        // Do nothing
        void OP_NULL();

        // CLS: Clear the display
        void OP_00E0();

        // RET: Return from a subroutine
        void OP_00EE();

        // JP addr: Jump to location nnn
        void OP_1nnn();

        // CALL addr: Call subroutine at nnn
        void OP_2nnn();

        // SE Vx, byte: Skip next instruction if Vx = kk
        void OP_3xkk();

        // SNE Vx, byte: Skip next instruction if Vx != kk
        void OP_4xkk();

        // SE Vx, Vy: Skip next instruction if Vx = Vy
        void OP_5xy0();

        // LD Vx, byte: Set Vx = kk
        void OP_6xkk();

        // ADD Vx, byte: Set Vx = Vx + kk
        void OP_7xkk();

        // LD Vx, Vy: Set Vx = Vy
        void OP_8xy0();

        // OR Vx, Vy: Set Vx = Vx OR Vy
        void OP_8xy1();

        // AND Vx, Vy: Set Vx = Vx AND Vy
        void OP_8xy2();

        // XOR Vx, Vy: Set Vx = Vx XOR Vy
        void OP_8xy3();

        // ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry
        void OP_8xy4();

        // SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow
        void OP_8xy5();

        // SHR Vx {, Vy}: Set Vx = Vx SHR 1
        void OP_8xy6();

        // SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow
        void OP_8xy7();

        // SHL Vx {, Vy}: Set Vx = Vx SHL 1
        void OP_8xyE();

        // SNE Vx, Vy: Skip next instruction if Vx != Vy
        void OP_9xy0();

        // LD I, addr: Set I = nnn
        void OP_Annn();

        // JP V0, addr: Jump to location nnn + V0
        void OP_Bnnn();

        // RND Vx, byte: Set Vx = random byte AND kk
        void OP_Cxkk();

        // DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void OP_Dxyn();

        // SKP Vx: Skip next instruction if key with the value of Vx is pressed
        void OP_Ex9E();

        // SKNP Vx: Skip next instruction if key with the value of Vx is not pressed
        void OP_ExA1();

        // LD Vx, DT: Set Vx = delay timer value
        void OP_Fx07();

        // LD Vx, K: Wait for a key press, store the value of the key in Vx
        void OP_Fx0A();

        // LD DT, Vx: Set delay timer = Vx
        void OP_Fx15();

        // LD ST, Vx: Set sound timer = Vx
        void OP_Fx18();

        // ADD I, Vx: Set I = I + Vx
        void OP_Fx1E();

        // LD F, Vx: Set I = location of sprite for digit Vx
        void OP_Fx29();

        // LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2
        void OP_Fx33();

        // LD [I], Vx: Store registers V0 through Vx in memory starting at location I
        void OP_Fx55();

        // LD Vx, [I]: Read registers V0 through Vx from memory starting at location I
        void OP_Fx65();

        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;

        typedef void (Chip8::*Chip8Func)();
        Chip8Func table[0xF+1];
        Chip8Func table0[0xE+1];
        Chip8Func table8[0xE+1];
        Chip8Func tableE[0xE+1];
        Chip8Func tableF[0x65+1];
};
