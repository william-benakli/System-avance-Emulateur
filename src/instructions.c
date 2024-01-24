#include "instructions.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

void printGraphics(bool display[]) {
    system("clear");
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            printf("%c", display[y * DISPLAY_WIDTH + x] ? '#' : '.');
        }
        printf("\n");
    }
}
