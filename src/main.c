#include "main.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double bufferTime = 0.0f;

void update(double delta) {
    bufferTime += delta;
    // update every 1/60th of a second
    if (bufferTime < 1. / 500.) {
        return;
    }
    bufferTime = 0.0f;

    uint16_t opcode = fetch();
    struct nibble data = decode(opcode);
    execute(data);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return 1;
    }
    initialize();
    load_rom(argv[1]);
    float delta = 0.0;
    while (true) {
        struct timespec start, end;
        clock_gettime(4, &start);
        update(delta);
        clock_gettime(4, &end);
        delta = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    }
    return 0;
}
