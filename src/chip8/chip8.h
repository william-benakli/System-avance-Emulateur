#ifndef CHIP8_H
#define CHIP8_H

#include "stack.h"
#include "quirks.h"

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define FRAMERATE 60.0 // 60 Hz

#define MEMORY_SIZE 4 * 1024
#define FONT_START 0x50
#define ROM_START 0x200

typedef struct Chip8 {
    uint8_t memory[MEMORY_SIZE]; // Memory: CHIP-8 has direct access to up to 4 kilobytes of RAM
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT]; // Display: 64 x 32 pixels (or 128 x 64 for SUPER-CHIP) monochrome, ie. black or white
    uint16_t program_counter; // A program counter, often called just “PC”, which points at the current instruction in memory
    uint16_t index_register; // One 16-bit index register called “I” which is used to point at locations in memory
    stack stack; // A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
    uint8_t delay; // An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
    uint8_t sound; // An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
    uint8_t V[16]; // 16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie. 0 through 15 in decimal, called V0 through VF

    bool pressed_keys[16]; // 16 keys, which are the hexadecimal digits 0 through F

    uint16_t instructions_per_second; // The number of instructions to execute per second
    machine_type_t machine;
} chip8_t;

typedef struct nibble {
    uint16_t opcode;
    uint8_t t; // The first nibble. Used to look up one of the 35 opcodes.
    uint8_t x; // The second nibble. Used to look up one of the 16 registers (VX) from V0 through VF.
    uint8_t y; // The third nibble. Also used to look up one of the 16 registers (VY) from V0 through VF.
    uint8_t n; // The fourth nibble. A 4-bit number.
    uint8_t nn; // The second byte (third and fourth nibbles). An 8-bit immediate number.
    uint16_t nnn; // The second, third and fourth nibbles. A 12-bit immediate memory address.
} nibble;

void initialize_chip8(chip8_t *chip8);
bool load_rom(chip8_t *chip8, const char *path);
void unload_rom(chip8_t *chip8);
void run(chip8_t *chip8, const uint32_t n);
void update_cycle(chip8_t *chip8);
void update_timers(chip8_t *chip8);

uint16_t fetch(chip8_t *chip8);
nibble decode(uint16_t opcode);

#endif
