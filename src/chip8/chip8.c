#include "chip8.h"
#include "stack.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void execute(chip8_t *chip8, nibble data);

// helper functions
bool is_key_pressed(bool keys[16]);
uint8_t get_pressed_key(bool keys[16]);
void get_bits_from_byte(uint8_t byte, bool bits[8]);

void initialize_chip8(chip8_t *chip8) {
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        chip8->memory[i] = 0;
    }
    chip8->memory[0] = 0x10;
    chip8->memory[1] = 0x00;
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
    memcpy(&chip8->memory[FONT_START], font, sizeof(font));
    for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        chip8->display[i] = false;
    }
    chip8->program_counter = 0;
    chip8->index_register = 0;
    stack_init(&chip8->stack);
    chip8->delay = 0;
    chip8->sound = 0;
    for (size_t i = 0; i < 16; i++) {
        chip8->V[i] = 0;
        chip8->pressed_keys[i] = false;
    }
    chip8->instructions_per_second = 700;
    chip8->machine = SUPER_CHIP;
    srand(time(NULL));
}

bool load_rom(chip8_t *chip8, const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open rom file");
        return false;
    }
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Failed to stat rom file");
        return false;
    }

    if (read(fd, chip8->memory + ROM_START, st.st_size) == -1) {
        perror("Failed to read rom file");
        return false;
    }
    if (close(fd) == -1) {
        perror("Failed to close rom file");
        return false;
    }

    chip8->program_counter = ROM_START;
    return true;
}

void unload_rom(chip8_t *chip8) {
    for (size_t i = ROM_START; i < MEMORY_SIZE; i++) {
        chip8->memory[i] = 0;
    }
    for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        chip8->display[i] = false;
    }
    chip8->program_counter = 0;
    chip8->index_register = 0;
    stack_clear(&chip8->stack);
    chip8->delay = 0;
    chip8->sound = 0;
    for (size_t i = 0; i < 16; i++) {
        chip8->V[i] = 0;
        chip8->pressed_keys[i] = false;
    }
}

void run(chip8_t *chip8, const uint32_t n) {
    for (size_t i = 0; i < n; i++) {
        update_cycle(chip8);
    }
}

void update_cycle(chip8_t *chip8) {
    uint16_t opcode = fetch(chip8);
    nibble data = decode(opcode);
    execute(chip8, data);
}

void update_timers(chip8_t *chip8) {
    if (chip8->delay > 0) {
        chip8->delay--;
    }
    if (chip8->sound > 0) {
        chip8->sound--;
    }
}

uint16_t fetch(chip8_t *chip8) {
    uint8_t highByte, lowByte;
    memcpy(&highByte, chip8->memory + chip8->program_counter, sizeof(uint8_t));
    memcpy(&lowByte, chip8->memory + chip8->program_counter + 1, sizeof(uint8_t));
    chip8->program_counter += 2;
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

void execute(chip8_t *chip8, nibble data) {
    // printf("Handling opcode: %04X...\n", data.opcode);
    switch (data.t) {
    case 0x0:
        if (data.nnn == 0x0E0) { // 00E0: Clear screen
            for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
                chip8->display[i] = false;
            }
        } else if (data.nnn == 0x0EE) { // 00EE: Return from subroutine
            chip8->program_counter = stack_pop(&chip8->stack);
        } else {
            errno = EINVAL;
            fprintf(stderr, "Unknown opcode %04X\n", data.opcode);
            exit(EXIT_FAILURE);
        }
        break;
    case 0x1: // 1NNN: Jump
        chip8->program_counter = data.nnn;
        break;
    case 0x2: // 2NNN: Call subroutine
        stack_push(&chip8->stack, chip8->program_counter);
        chip8->program_counter = data.nnn;
        break; 
    case 0x3: // 3XNN: Skip next instruction if VX == NN
        if (chip8->V[data.x] == data.nn) {
            chip8->program_counter += 2;
        }
        break;
    case 0x4: // 4XNN: Skip next instruction if VX != NN
        if (chip8->V[data.x] != data.nn) {
            chip8->program_counter += 2;
        }
        break;
    case 0x5: // 5XY0: Skip next instruction if VX == VY
        if (chip8->V[data.x] == chip8->V[data.y]) {
            chip8->program_counter += 2;
        }
        break;
    case 0x6: // 6XNN: Set VX to NN
        chip8->V[data.x] = data.nn;
        break;
    case 0x7: // 7XNN: Add NN to VX
        chip8->V[data.x] += data.nn;
        break;
    case 0x8:
        switch (data.n) {
        case 0x0: // 8XY0: Sets VX to the value of VY
            chip8->V[data.x] = chip8->V[data.y];
            break;
        case 0x1: // 8XY1: Sets VX to VX or VY. (bitwise OR operation)
            chip8->V[data.x] |= chip8->V[data.y];
            if (chip8->machine == COSMAC_VIP) {
                chip8->V[0xF] = 0;
            }
            break;
        case 0x2: // 8XY2: Sets VX to VX and VY. (bitwise AND operation)
            chip8->V[data.x] &= chip8->V[data.y];
            if (chip8->machine == COSMAC_VIP) {
                chip8->V[0xF] = 0;
            }
            break;
        case 0x3: // 8XY3: Sets VX to VX xor VY
            chip8->V[data.x] ^= chip8->V[data.y];
            if (chip8->machine == COSMAC_VIP) {
                chip8->V[0xF] = 0;
            }
            break;
        case 0x4: { // 8XY4: Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not
            bool overflow = chip8->V[data.x] + chip8->V[data.y] > 255;
            chip8->V[data.x] += chip8->V[data.y];
            chip8->V[0xF] = overflow;
            break;
        }
        case 0x5: { // 8XY5: VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when there is not (i.e. VF set to 1 if VX >= VY and 0 if not)
            bool underflow = chip8->V[data.x] < chip8->V[data.y];
            chip8->V[data.x] -= chip8->V[data.y];
            chip8->V[0xF] = underflow ? 0 : 1;
            break;
        }
        case 0x6: { // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
            if (chip8->machine == COSMAC_VIP) {
                chip8->V[data.x] = chip8->V[data.y];
            }
            uint8_t shifted_out = chip8->V[data.x] & 0x1;
            chip8->V[data.x] >>= 1; // Shift the value of VX one bit to the right
            chip8->V[0xF] = shifted_out; // Set VF to 1 if the bit that was shifted out was 1, or 0 if it was 0
            break;
        }
        case 0x7: { // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX)
            bool underflow = chip8->V[data.y] < chip8->V[data.x];
            chip8->V[data.x] = chip8->V[data.y] - chip8->V[data.x];
            chip8->V[0xF] = underflow ? 0 : 1;
            break;
        }
        case 0xE: { // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
            if (chip8->machine == COSMAC_VIP) {
                chip8->V[data.x] = chip8->V[data.y];
            }
            uint8_t shifted_out = (chip8->V[data.x] >> 7) & 0x1;
            chip8->V[data.x] <<= 1;
            chip8->V[0xF] = shifted_out;
            break;
        }
        default:
            errno = EINVAL;
            fprintf(stderr, "Unknown opcode %04X\n", data.opcode);
            exit(EXIT_FAILURE);
        }
        break;
    case 0x9:// 9XY0: Skip next instruction if VX != VY
        if (chip8->V[data.x] != chip8->V[data.y]) {
            chip8->program_counter += 2;
        }
        break;
    case 0xA: // ANNN: Set I to NNN
        chip8->index_register = data.nnn;
        break;
    case 0xB: // BNNN: Jumps to the adress NNN + V0
        if (chip8->machine == COSMAC_VIP) {
            chip8->program_counter = data.nnn + chip8->V[0x0];
        } else if (chip8->machine == CHIP_48 || chip8->machine == SUPER_CHIP) {
            chip8->program_counter = data.nnn + chip8->V[data.x];
        }
        break;
    case 0xC: // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
        chip8->V[data.x] = rand() & data.nn;
        break;
    case 0xD: { // DXYN: Draw sprite
        uint8_t x = chip8->V[data.x] % 64; // Set the X coordinate to the value in VX modulo 64 (or, equivalently, VX & 63, where & is the binary AND operation)
        uint8_t y = chip8->V[data.y] % 32; // Set the Y coordinate to the value in VY modulo 32 (or VY & 31)
        chip8->V[0xF] = 0; // Set VF to 0
        for (int row = 0; row < data.n; row++) { // For N rows:
            uint8_t pixels = chip8->memory[chip8->index_register + row]; // Get the Nth byte of sprite data, counting from the memory address in the I register (I is not incremented)
            bool pixelsBitsRow[8];
            get_bits_from_byte(pixels, pixelsBitsRow);
            for (int col = 0; col < 8; col++) { // For each of the 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
                if (pixelsBitsRow[col]) { // bit is 1
                    int coord = (y + row) * DISPLAY_WIDTH + x + col;
                    if (chip8->display[coord]) { // If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on, turn off the pixel and set VF to 1
                        chip8->display[coord] = false;
                        chip8->V[0xF] = 1;
                    } else { // Or if the current pixel in the sprite row is on and the screen pixel is not, draw the pixel at the X and Y coordinates
                        chip8->display[coord] = true;
                    }
                }
                if (x + col >= DISPLAY_WIDTH - 1) { // If you reach the right edge of the screen, stop drawing this row
                    break;
                }
            }
            if (y + row >= DISPLAY_HEIGHT - 1) { // Stop if you reach the bottom edge of the screen
                break;
            }
        }
        break;
    }
    case 0xE:
        if (data.nn == 0x9E) { // EX9E: Skip next instruction if key with the value of VX is pressed
            if (chip8->pressed_keys[chip8->V[data.x]]) {
                chip8->program_counter += 2;
            }
        } else if (data.nn == 0xA1) { // EXA1: Skip next instruction if key with the value of VX isn't pressed
            if (!chip8->pressed_keys[chip8->V[data.x]]) {
                chip8->program_counter += 2;
            }
        } else {
            errno = EINVAL;
            fprintf(stderr, "Unknown opcode %04X\n", data.opcode);
            exit(EXIT_FAILURE);
        }
        break;
    case 0xF:
        switch (data.nn) {
        case 0x07: // FX07: Sets VX to the value of the delay timer
            chip8->V[data.x] = chip8->delay; 
            break;
        case 0x0A: // FX0A: A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event)
            chip8->program_counter -= 2; // default behaviour: repeat the action
            static bool buffer[16];
            if (chip8->machine == COSMAC_VIP) {  // On the original COSMAC VIP, the key was only registered when it was pressed and then released.
                bool key_up[16];
                for (size_t i = 0; i < 16; i++) { // Store the keys that were released
                    key_up[i] = buffer[i] && !chip8->pressed_keys[i];
                }
                if (!is_key_pressed(buffer)) { // If no key were pressed to begin with, store pressed keys and wait
                    // chip8->buffer = chip8->pressed_keys;
                    for (size_t i = 0; i < 16; i++) {
                        buffer[i] = chip8->pressed_keys[i];
                    }
                    break;
                } else if (is_key_pressed(key_up)) {
                    chip8->program_counter += 2;
                    for (size_t i = 0; i < 16; i++) {
                        buffer[i] = false;
                    }
                    uint8_t key = get_pressed_key(key_up);
                    chip8->V[data.x] = key;
                } else {
                    // chip8->buffer16 |= chip8->pressed_keys;
                    for (size_t i = 0; i < 16; i++) {
                        buffer[i] = buffer[i] || chip8->pressed_keys[i];
                    }
                }
            } else if (is_key_pressed(chip8->pressed_keys)) { // Just wait for a key to be pressed
                chip8->program_counter += 2;
                uint8_t key = get_pressed_key(chip8->pressed_keys);
                chip8->V[data.x] = key;
            }
            break;
            case 0x15: // FX15: Sets the delay timer to VX
                chip8->delay = chip8->V[data.x];
                break;
            case 0x18: // FX18: Sets the sound timer to VX
                chip8->sound = chip8->V[data.x];
                break;
            case 0x1E: // FX1E: Adds VX to I. VF is not affected
                if (chip8->machine == AMIGA) {
                    chip8->V[0xF] = chip8->index_register < 0x0FFF && chip8->index_register + chip8->V[data.x] >= 0x1000 ? 1 : 0;
                }
                chip8->index_register += chip8->V[data.x];
                break;
            case 0x29: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
                if (chip8->machine == COSMAC_VIP) {
                    chip8->index_register = FONT_START + (chip8->V[data.x] & 0xF) * 5;
                } else {
                    chip8->index_register = FONT_START + chip8->V[data.x] * 5;
                }
                break;
            case 0x33: // FX33: Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
                chip8->memory[chip8->index_register] = chip8->V[data.x] / 100;
                chip8->memory[chip8->index_register + 1] = chip8->V[data.x] / 10 % 10;
                chip8->memory[chip8->index_register + 2] = chip8->V[data.x] % 10;
                break;
            case 0x55: // FX55: Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
                for (size_t i = 0; i <= data.x; i++) {
                    chip8->memory[chip8->index_register + i] = chip8->V[i];
                }
                if (chip8->machine == COSMAC_VIP) {
                    chip8->index_register += data.x + 1;
                }
                break;
            case 0x65: // FX65: Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified
                for (size_t i = 0; i <= data.x; i++) {
                    chip8->V[i] = chip8->memory[chip8->index_register + i];
                }
                if (chip8->machine == COSMAC_VIP) {
                    chip8->index_register += data.x + 1;
                }
                break;
            default:
                errno = EINVAL;
                fprintf(stderr, "Unknown opcode %04X\n", data.opcode);
                exit(EXIT_FAILURE);
        }
        break;
    default:
        errno = EINVAL;
        fprintf(stderr, "Unknown opcode %04X\n", data.opcode);
        exit(EXIT_FAILURE);
    }
}

bool is_key_pressed(bool keys[16]) {
    for (size_t i = 0; i < 16; i++) {
        if (keys[i]) {
            return true;
        }
    }
    return false;
}

uint8_t get_pressed_key(bool keys[16]) {
    for (size_t i = 0; i < 16; i++) {
        if (keys[i]) {
            return i;
        }
    }
    return 0xFF;
}

void get_bits_from_byte(uint8_t byte, bool bits[8]) {
    for (int i = 0; i < 8; i++) {
        bits[i] = (byte >> (7 - i)) & 1;
    }
}
