#include "chip8.h"
#include "instructions.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct Chip8 chip8;

void initialize() {
    uint8_t font[5 * 16] = {
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
    memcpy(&chip8.memory[0x50], font, sizeof(font));
    stack_init(&chip8.stack);
}

void load_rom(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    int s = fstat(fd, &st);
    if (s == -1) {
        fprintf(stderr, "Failed to stat file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(fd, chip8.memory + ROM_START, st.st_size);
    if (bytes_read == -1) {
        fprintf(stderr, "Failed to read file: %s\n", path);
        exit(EXIT_FAILURE);
    }
    int c = close(fd);
    if (c == -1) {
        fprintf(stderr, "Failed to close file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    chip8.program_counter = ROM_START - 1;
    printf("Loaded ROM: %s\n", path);

}

uint16_t fetch() {
    uint16_t opcode;
    memcpy(&opcode, chip8.memory + chip8.program_counter, sizeof(uint16_t));
    chip8.program_counter += 2;
    return opcode;
}

nibble decode(uint16_t opcode) {
    nibble data = {
        .opcode = opcode,
        .t = opcode >> 12,
        .x = (opcode >> 8) & 0x0F,
        .y = (opcode >> 4) & 0x0F,
        .n = opcode & 0x0F,
        .nn = opcode & 0xFF,
        .nnn = opcode & 0xFFF
    };
    return data;
}

void execute(nibble data) {
    printf("Handling opcode: %04X...\n", data.opcode);
    switch (data.t) {
        case 0x0:
            if (data.nnn == 0x0E0) { // 00E0: Clear screen
                for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
                    chip8.display[i] = false;
                }
                printGraphics(chip8.display);
            } else if (data.nnn == 0x0EE) { // 00EE: Return from subroutine
                chip8.program_counter = stack_pop(&chip8.stack);
            }
            break;
        case 0x1: // 1NNN: Jump
            chip8.program_counter = data.nnn;
            break;
        case 0x2: // 2NNN: Call subroutine
            stack_push(&chip8.stack, chip8.program_counter);
            chip8.program_counter = data.nnn;
            break;
        case 0x6: // 6XNN: Set VX to NN
            chip8.V[data.x] = data.nn;
            break;
        case 0x7: // 7XNN: Add NN to VX
            chip8.V[data.x] += data.nn;
            break;
        case 0xA: // ANNN: Set I to NNN
            chip8.index_register = data.nnn;
            break;
        case 0xD: // DXYN: Draw sprite
            uint8_t x = chip8.V[data.x] % 64; // Set the X coordinate to the value in VX modulo 64 (or, equivalently, VX & 63, where & is the binary AND operation)
            uint8_t y = chip8.V[data.y] % 32; // Set the Y coordinate to the value in VY modulo 32 (or VY & 31)
            chip8.V[0xF] = 0; // Set VF to 0
            for (int i = 0; i < data.n; i++) { // For N rows:
                uint8_t pixels = chip8.memory[chip8.index_register + i]; // Get the Nth byte of sprite data, counting from the memory address in the I register (I is not incremented)
                unsigned int mask;
                for (mask = 0x80; mask != 0; mask >>= 1) { // For each of the 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
                    if (pixels & mask) { // bit is 1
                        int coord = y * DISPLAY_WIDTH + x;
                        // If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on, turn off the pixel and set VF to 1
                        // Or if the current pixel in the sprite row is on and the screen pixel is not, draw the pixel at the X and Y coordinates
                        chip8.display[coord] = !chip8.display[coord];
                        if (chip8.display[coord]) {
                            chip8.V[0xF] = 1;
                        }
                    }
                    if (x >= DISPLAY_WIDTH - 1) { // If you reach the right edge of the screen, stop drawing this row
                        break;
                    }
                    x++; // Increment X (VX is not incremented)
                }
                y++; // Increment Y (VY is not incremented)
                if (y <= 0) { // Stop if you reach the bottom edge of the screen
                    break;
                }
            }
            printGraphics(chip8.display);
            break;
        default:
            fprintf(stderr, "Unhandled operation: %04X\n", data.opcode);
            exit(EXIT_FAILURE);
    }
}
