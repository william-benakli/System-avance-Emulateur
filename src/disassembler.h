#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "chip8.h"

#define DIR "disassembled_roms/"

void file_stem(char *dest, char *src);
void log_op(nibble data, int fd, int offset);
int main(int argc, char **argv);

#endif
