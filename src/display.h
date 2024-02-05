#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

int initialize_display();
void update_display(long double delta);
void close_display();

#endif
