#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

void signalHandler(int signal);
void update(double delta);
int main(int argc, char **argv);

#endif
