#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <time.h>

void signalHandler(int signal);
void update(long double delta);
int main(int argc, char **argv);

#endif
