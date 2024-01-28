#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

#include <SDL2/SDL.h>

void initializeDisplay();
void handleInput();
void refreshFrame();
void closeDisplay();

#endif
