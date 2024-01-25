#include "instructions.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

void printGraphics(bool display[]) {
    system("clear");
    char displayStr[DISPLAY_WIDTH * DISPLAY_HEIGHT + DISPLAY_HEIGHT];
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        int index = i + i / DISPLAY_WIDTH;
        displayStr[index] = display[i] ? '#' : '.';
        if ((i + 1) % DISPLAY_WIDTH == 0) {
            displayStr[index + 1] = '\n';
        }
    }
    printf("%s", displayStr);
}
