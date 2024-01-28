#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

#include <SDL2/SDL.h>

#define SCALING 1

#define PRIMARY_COLOR 255, 255, 255, 255 
#define SECONDARY_COLOR 0, 0, 0, 255

SDL_Window *window;
SDL_Renderer *renderer;

void initializeDisplay();
void handleInput();
void refreshFrame();
void closeDisplay();

#endif
