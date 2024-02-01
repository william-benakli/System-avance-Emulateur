#include "util.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

#define PIXEL_ON '#'
#define PIXEL_OFF ' '

void get_bits_from_byte(uint8_t byte, bool bits[8]) {
    for (int i = 0; i < 8; i++) {
        bits[i] = (byte >> (7 - i)) & 1;
    }
}

void printGraphics(bool display[]) {
    char displayStr[DISPLAY_WIDTH * DISPLAY_HEIGHT + DISPLAY_HEIGHT];
    for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        int index = i + i / DISPLAY_WIDTH;
        displayStr[index] = display[i] ? PIXEL_ON : PIXEL_OFF;
        if ((i + 1) % DISPLAY_WIDTH == 0) {
            displayStr[index + 1] = '\n';
        }
    }
    system("clear");
    printf("%s", displayStr);
}
