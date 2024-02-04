#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"
#include "stdint.h"

#include <stdbool.h>

int initialize_display();
void handle_inputs(bool keys[16]);
void handle_sound(uint16_t sound_delay);
void refresh_frame();
void close_display();

#endif
