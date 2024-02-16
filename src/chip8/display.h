#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

int initialize_display();
void handle_inputs(bool pressed_keys[16]);
void handle_sound(uint16_t sound_delay);
void refresh_frame(bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT]);
void close_display();

#endif
