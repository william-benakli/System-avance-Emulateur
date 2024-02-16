#include "chip8.h"
#include "display.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool running = true;

void signalHandler(int signal);
struct timespec diff(struct timespec start, struct timespec end);
long double timespec_to_s(struct timespec time);
void update(chip8_t *chip8, long double delta);
int main(int argc, char **argv);

void signalHandler(int signal) {
    running = false;
}

struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec time_diff;
    if (end.tv_nsec - start.tv_nsec < 0)  {
        time_diff.tv_sec = end.tv_sec - start.tv_sec - 1;
        time_diff.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        time_diff.tv_sec = end.tv_sec - start.tv_sec;
        time_diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return time_diff;
}

long double timespec_to_s(struct timespec time) {
    return time.tv_sec + time.tv_nsec * 1e-9;
}

void update(chip8_t *chip8, long double delta) {
    static long double bufferTime, bufferTime2;
    bufferTime += delta;
    bufferTime2 += delta;
    if (bufferTime >= 1.0 / FRAMERATE) {
        bufferTime -= 1.0 / FRAMERATE;
        // chip8
        update_timers(chip8);
        // display
        handle_inputs(chip8->pressed_keys);
        handle_sound(chip8->sound);
        refresh_frame(chip8->display);
    }
    if (bufferTime2 >= 1.0 / chip8->instructions_per_second) {
        bufferTime2 -= 1.0 / chip8->instructions_per_second;
        update_cycle(chip8);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return EXIT_FAILURE;
    }
    chip8_t chip8;
    initialize_chip8(&chip8);
    load_rom(&chip8, argv[1]);
    if (initialize_display(&running) == -1) {
        return EXIT_FAILURE;
    }
    signal(SIGINT, signalHandler);
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    while (running) {
        struct timespec last = now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        struct timespec diff_time = diff(last, now);
        long double delta = timespec_to_s(diff_time);
        update(&chip8, delta);
    }
    close_display();
    return EXIT_SUCCESS;
}
