#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

void initializeDisplay();
void refreshFrame();
void closeDisplay();

#endif
