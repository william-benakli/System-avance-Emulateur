#include "instructions.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

void printGraphics(bool display[]) {
    system("clear");
    /* char* displayStr = malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT + DISPLAY_HEIGHT);
    int offset = 0;
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        int index = i + offset;
        displayStr[index] = display[i] ? '#' : '.';
        if (i != 0 && i % DISPLAY_WIDTH == 0) {
            displayStr[index + 1] = '\n';
            offset++;
        }
    }
    printf("%s", displayStr);
    free(displayStr); */
    
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            printf("%c ", display[y * DISPLAY_WIDTH + x] ? '#' : '.');
        }
        printf("\n");
    }
}
