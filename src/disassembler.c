#include "disassembler.h"
#include "chip8.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

void log_op(nibble data, int fd, int offset) {
    dprintf(fd, "%04X\t", offset + ROM_START);
    switch (data.t) {
        case 0x0:
            if (data.nnn == 0x0E0) {
                dprintf(fd, "CLEAR_SCREEN\n");
                return;
            }
        case 0x1: // 1NNN: Jump
            dprintf(fd, "JUMP %02X\n", data.nnn);
            return;
        case 0x6: // 6XNN: Set VX to NN
            dprintf(fd, "SET V%x, %x\n", data.x, data.nn);
            return;
        case 0x7: // 7XNN: Add NN to VX
            dprintf(fd, "ADD V%x, %x\n", data.x, data.nn);
            return;
        case 0xA: // ANNN: Set I to NNN
            dprintf(fd, "SET I, %x\n", data.nnn);
            return;
        case 0xD: // DXYN: Draw sprite
            dprintf(fd, "DRAW V%x, V%x, %x\n", data.x, data.y, data.n);
            return;
        default:
            break;
    }

    dprintf(fd, "Unknown opcode: %04X\n", data.opcode);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char asm_filename[256];
    rom_filename_to_asm_filename(asm_filename, argv[1]);

    int asm_fd = open(asm_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (asm_fd == -1) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }
    int rom_fd = open(argv[1], O_RDONLY);
    if (rom_fd == -1) {
        perror("Error opening ROM file");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (fstat(rom_fd, &st) == -1) {
        perror("Error getting log file stats");
        exit(EXIT_FAILURE);
    }
    int offset = 0;
    while (offset < st.st_size) {
        uint16_t opcode;
        uint8_t highByte, lowByte;
        if (read(rom_fd, &highByte, sizeof(uint8_t)) == -1) {
            perror("Error reading log file");
            exit(EXIT_FAILURE);
        }
        if (read(rom_fd, &lowByte, sizeof(uint8_t)) == -1) {
            perror("Error reading log file");
            exit(EXIT_FAILURE);
        }
        opcode = (highByte << 8) | lowByte;

        nibble data = decode(opcode);

        log_op(data, asm_fd, offset);
        offset += 2;
    }
    if (close(asm_fd) == -1) {
        perror("Error closing log file");
        exit(EXIT_FAILURE);
    }
    printf("Disassembled ROM to %s\n", asm_filename);
    return EXIT_SUCCESS;
}
