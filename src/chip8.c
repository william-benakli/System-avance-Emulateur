#include "chip8.h"
#include "util.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct Chip8 chip8;

void initializeChip8() {
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
    memcpy(&chip8.memory[FONT_START], font, sizeof(font));
    stack_init(&chip8.stack);
}

void load_rom(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open rom file");
        exit(EXIT_FAILURE);
    }
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Failed to stat rom file");
        exit(EXIT_FAILURE);
    }

    if (read(fd, chip8.memory + ROM_START, st.st_size) == -1) {
        perror("Failed to read rom file");
        exit(EXIT_FAILURE);
    }
    if (close(fd) == -1) {
        perror("Failed to close rom file");
        exit(EXIT_FAILURE);
    }

    chip8.program_counter = ROM_START;
    printf("Loaded ROM: %s\n", path);

}

uint16_t fetch() {
    uint8_t highByte, lowByte;
    memcpy(&highByte, chip8.memory + chip8.program_counter, sizeof(uint8_t));
    memcpy(&lowByte, chip8.memory + chip8.program_counter + 1, sizeof(uint8_t));
    chip8.program_counter += 2;
    return (highByte << 8) | lowByte;
}

nibble decode(uint16_t opcode) {
    nibble data = {
        .opcode = opcode,
        .t = opcode >> 12,
        .x = opcode >> 8 & 0xF,
        .y = opcode >> 4 & 0xF,
        .n = opcode & 0xF,
        .nn = opcode & 0xFF,
        .nnn = opcode & 0xFFF
    };
    return data;
}

void execute(nibble data) {
    // printf("Handling opcode: %04X...\n", data.opcode);
    switch (data.t) {
        case 0x0:
            if (data.nnn == 0x0E0) { // 00E0: Clear screen
                for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
                    chip8.display[i] = false;
                }
                printGraphics(chip8.display);
            }
            break;
        case 0x1: // 1NNN: Jump
            chip8.program_counter = data.nnn;
            break;
        /* case 0x2: // 2NNN: Call subroutine
            stack_push(&chip8.stack, chip8.program_counter);
            chip8.program_counter = data.nnn;
            break; */
        case 0x6: // 6XNN: Set VX to NN
            chip8.V[data.x] = data.nn;
            break;
        case 0x7: // 7XNN: Add NN to VX
            chip8.V[data.x] += data.nn;
            break;
        case 0xA: // ANNN: Set I to NNN
            chip8.index_register = data.nnn;
            break;
        case 0xD: { // DXYN: Draw sprite
            uint8_t x = chip8.V[data.x] % 64; // Set the X coordinate to the value in VX modulo 64 (or, equivalently, VX & 63, where & is the binary AND operation)
            uint8_t y = chip8.V[data.y] % 32; // Set the Y coordinate to the value in VY modulo 32 (or VY & 31)
            chip8.V[0xF] = 0; // Set VF to 0
            for (int row = 0; row < data.n; row++) { // For N rows:
                uint8_t pixels = chip8.memory[chip8.index_register + row]; // Get the Nth byte of sprite data, counting from the memory address in the I register (I is not incremented)
                bool pixelsBitsRow[8];
                get_bits_from_byte(pixels, pixelsBitsRow);
                for (int col = 0; col < 8; col++) { // For each of the 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
                    if (pixelsBitsRow[col]) { // bit is 1
                        int coord = (y + row) * DISPLAY_WIDTH + (x + col);
                        if (chip8.display[coord]) { // If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on, turn off the pixel and set VF to 1
                            chip8.display[coord] = false;
                            chip8.V[0xF] = 1;
                        } else { // Or if the current pixel in the sprite row is on and the screen pixel is not, draw the pixel at the X and Y coordinates
                            chip8.display[coord] = true;
                        }
                    }
                    /* if (x >= DISPLAY_WIDTH - 1) { // If you reach the right edge of the screen, stop drawing this row
                        break;
                    } */
                }
                /* if (y <= 0 || y >= DISPLAY_HEIGHT - 1) { // Stop if you reach the bottom edge of the screen
                    break;
                } */
            }
            printGraphics(chip8.display);
            break;
        }
        default:
            errno = EINVAL;
            perror("Unknown opcode");
            exit(EXIT_FAILURE);
    }
}
