#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"
#include "stdint.h"

#include <SDL2/SDL.h>

void initialize_display();
uint16_t handle_inputs();
void refresh_frame();
void close_display();

#endif
