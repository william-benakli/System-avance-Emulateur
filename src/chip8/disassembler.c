#include "chip8.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIR "disassembled_roms/"

void file_stem(char *dest, char *src);
void rom_filename_to_asm_filename(char *dest, char *rom_filename);
int log_op(char* dest, nibble data, int offset);
int main(int argc, char **argv);

void file_stem(char *dest, char *src) {
    int filename_len = strlen(src);
    int i = filename_len - 1;
    while (src[i] != '.') {
        i--;
    }

    int j = i;
    while (src[j] != '/') {
        j--;
    }

    memcpy(dest, src + j + 1, i - j - 1);
    dest[i - j - 1] = '\0';
}

void rom_filename_to_asm_filename(char *dest, char *rom_filename) {
    char stem[256];
    file_stem(stem, rom_filename);

    strcpy(dest, DIR);
    strcat(dest, stem);
    strcat(dest, ".asm");
}

int log_op(char* dest, nibble data, int offset) {
    sprintf(dest, "%04X\t", offset + ROM_START);
    char* ptr = dest + strlen(dest);
    switch (data.t) {
    case 0x0:
        if (data.nnn == 0x0E0) return sprintf(ptr, "CLEAR_SCREEN\n"); // 00E0: Clear screen
        else if (data.nnn == 0x0EE) return sprintf(ptr, "RETURN_SUB\n"); // 00EE: Return from subroutine
    case 0x1: return sprintf(ptr, "JUMP %03X\n", data.nnn); // 1NNN: Jump
    case 0x2: return sprintf(ptr, "CALL_SUBROUTINE\n"); // 2NNN: Call subroutine
    case 0x3: return sprintf(ptr, "SKIP_IF V%01X == %02X\n", data.x, data.nn); // 3XNN: Skip next instruction if VX == NN
    case 0x4: return sprintf(ptr, "SKIP_IF V%01X != %02X\n", data.x, data.nn); // 4XNN: Skip next instruction if VX != NN
    case 0x5: return sprintf(ptr, "SKIP_IF V%01X == V%01X\n", data.x, data.y); // 5XY0: Skip next instruction if VX == VY
    case 0x6: return sprintf(ptr, "SET V%X, %02X\n", data.x, data.nn); // 6XNN: Set VX to NN
    case 0x7: return sprintf(ptr, "SET V%X, %X + %02X\n", data.x, data.x, data.nn); // 7XNN: Add NN to VX
    case 0x8:
        switch (data.n) {
        case 0x0: return sprintf(ptr, "SET V%X, V%X\n", data.x, data.y); // 8XY0: Sets VX to the value of VY
        case 0x1: return sprintf(ptr, "SET V%X, V%X | V%X\n", data.x, data.x, data.y); // 8XY1: Sets VX to VX or VY. (bitwise OR operation)
        case 0x2: return sprintf(ptr, "SET V%X, V%X & V%X\n", data.x, data.x, data.y); // 8XY2: Sets VX to VX and VY. (bitwise AND operation)
        case 0x3: return sprintf(ptr, "SET V%X, V%X ^ V%X\n", data.x, data.x, data.y); // 8XY3: Sets VX to VX xor VY
        case 0x4: return sprintf(ptr, "SET V%X, V%X + V%X\n", data.x, data.x, data.y); // 8XY4: Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not
        case 0x5: return sprintf(ptr, "SET V%X, V%X - V%X\n", data.x, data.x, data.y); // 8XY5: VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when there is not (i.e. VF set to 1 if VX >= VY and 0 if not)
        case 0x6: return sprintf(ptr, "SET VF, V%X >> 1\n", data.x); // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
        case 0x7: return sprintf(ptr, "SET V%X, V%X - V%X\n", data.x, data.x, data.y); // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX)
        case 0xE: return sprintf(ptr, "SET VF, V%X << 1\n", data.x); // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
        }
    case 0x9: return sprintf(ptr, "SKIP_IF V%X != V%X\n", data.x, data.y); // 9XY0: Skip next instruction if VX != VY
    case 0xA: return sprintf(ptr, "SET I, %3X\n", data.nnn); // ANNN: Set I to NNN
    case 0xB: return sprintf(ptr, "JUMP %3X + V%X\n", data.nnn, data.x); // BNNN: Jumps to the adress NNN + V0
    case 0xC: return sprintf(ptr, "SET V%X, rand & %2X\n", data.x, data.nn);; // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
    case 0xD: return sprintf(ptr, "DRAW V%X, V%X, %X\n", data.x, data.y, data.n); // DXYN: Draw sprite
    case 0xE:
        if (data.nn == 0x9E) return sprintf(ptr, "SKIP_IF key_V%X_pressed\n", data.x); // EX9E: Skip next instruction if key with the value of VX is pressed
        else if (data.nn == 0xA1) return sprintf(ptr, "SKIP_IF !key_V%X_pressed\n", data.x); // EXA1: Skip next instruction if key with the value of VX isn't pressed
    case 0xF:
        switch (data.nn) {
        case 0x07: return sprintf(ptr, "SET V%X, delay_timer\n", data.x); // FX07: Sets VX to the value of the delay timer
        case 0x0A: return sprintf(ptr, "WAIT_KEY V%X\n", data.x); // FX0A: A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event)
        case 0x15: return sprintf(ptr, "SET delay_timer, V%X\n", data.x); // FX15: Sets the delay timer to VX
        case 0x18: return sprintf(ptr, "SET sound_timer, V%X\n", data.x); // FX18: Sets the sound timer to VX
        case 0x1E: return sprintf(ptr, "SET I, V%X\n", data.x); // FX1E: Adds VX to I. VF is not affected
        case 0x29: return sprintf(ptr, "SET I, %X\n", FONT_START); // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
        case 0x33: return sprintf(ptr, "STORE_BCD memory+I, V%X\n", data.x) ; // FX33: Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
        case 0x55: return sprintf(ptr, "STORE memory+I, V0..V%X\n", data.x); // FX55: Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
        case 0x65: return sprintf(ptr, "STORE V0..V%X, memory+I\n", data.x);; // FX65: Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified
        }
    }
    return sprintf(ptr, "Unknown opcode: %04X\n", data.opcode);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int rom_fd = open(argv[1], O_RDONLY, S_IRUSR);
    if (rom_fd == -1) {
        perror("Error opening ROM file");
        return EXIT_FAILURE;
    }
    struct stat st;
    if (fstat(rom_fd, &st) == -1) {
        perror("Error getting rom file stats");
        return EXIT_FAILURE;
    }
    uint8_t rom_content_buffer[st.st_size];
    if (read(rom_fd, rom_content_buffer, st.st_size) == -1) {
        perror("Error reading rom file");
        return EXIT_FAILURE;
    }
    chip8_t chip8;
    load_rom(&chip8, argv[1]);

    char file_content[st.st_size * 100];
    int offset = 0;
    int file_offset = 0;
    while (offset < st.st_size) {
        char buffer[256];
        uint16_t opcode = fetch(&chip8);
        nibble data = decode(opcode);
        if (log_op(buffer, data, offset) == -1) {
            perror("Error logging opcode");
            return EXIT_FAILURE;
        }
        offset += 2;
        memcpy(file_content + file_offset, buffer, strlen(buffer));
        file_offset += strlen(buffer);
    }
    if (close(rom_fd) == -1) {
        perror("Error closing rom file");
        return EXIT_FAILURE;
    }
    
    char asm_filename[256];
    rom_filename_to_asm_filename(asm_filename, argv[1]);
    if (mkdir(DIR, S_IRWXU) == -1 && errno != EEXIST) {
        perror("Error creating disassembled roms directory");
        return EXIT_FAILURE;
    }
    int asm_fd = open(asm_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
    if (asm_fd == -1) {
        perror("Error opening log file");
        return EXIT_FAILURE;
    }
    if (write(asm_fd, file_content, strlen(file_content)) == -1) {
        perror("Error writing to log file");
        return EXIT_FAILURE;
    }
    if (close(asm_fd) == -1) {
        perror("Error closing asm file");
        return EXIT_FAILURE;
    }
    printf("Disassembled ROM to %s\n", asm_filename);
    return EXIT_SUCCESS;
}
