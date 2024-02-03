#include "emulator.h"
#include "chip8.h"
#include "display.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double bufferTime = 0.0f;
double bufferTime2 = 0.0f;

void signalHandler(int signal) {
    close_display();
    exit(EXIT_SUCCESS);
}

void update(double delta) {
    bufferTime += delta;
    bufferTime2 += delta;
    // update every 1/60th of a second
    if (bufferTime >= 1. / FRAMERATE) {
        bufferTime -= 1. / FRAMERATE;
        clock_timers();
        set_pressed_keys(handle_inputs());
        refresh_frame();
    }
    // update every 1/500th of a second
    if (bufferTime2 >= 1. / CLOCK_SPEED) {
        bufferTime2 -= 1. / CLOCK_SPEED;
        clock_cycle();
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return EXIT_FAILURE;
    }
    initialize_chip8();
    initialize_display();
    signal(SIGINT, signalHandler);
    load_rom(argv[1]);
    float delta = 0.0;
    while (true) {
        struct timespec start, end;
        clock_gettime(4, &start);
        update(delta);
        clock_gettime(4, &end);
        delta = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    }
    return EXIT_SUCCESS;
}
