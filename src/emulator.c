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
void update(long double delta);
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

void update(long double delta) {
    update_chip8(delta);
    update_display(delta);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return EXIT_FAILURE;
    }
    initialize_chip8();
    if (initialize_display(&running) == -1) {
        return EXIT_FAILURE;
    }
    signal(SIGINT, signalHandler);
    load_rom(argv[1]);
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    while (running) {
        struct timespec last = now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        struct timespec diff_time = diff(last, now);
        long double delta = timespec_to_s(diff_time);
        update(delta);
    }
    close_display();
    return EXIT_SUCCESS;
}
