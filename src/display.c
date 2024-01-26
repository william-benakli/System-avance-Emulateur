#include "display.h"

#include <stdio.h>

void initializeDisplay(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        closeDisplay();
        exit(EXIT_FAILURE);
    }
}

void refreshFrame() {
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    get_display(display);
    
    SDL_Color color = {0, 0, 0, 255};
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0)
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        closeDisplay();
        exit(EXIT_FAILURE);
    }
    SDL_RenderClear(renderer);
    color = (SDL_Color){255, 255, 255, 255};
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0)
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        closeDisplay();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        if (display[i]) {
            if (SDL_RenderDrawPoint(renderer, i % DISPLAY_WIDTH, i / DISPLAY_WIDTH) == -1) {
                fprintf(stderr, "Erreur SDL_RenderDrawPoint : %s", SDL_GetError());
                closeDisplay();
                exit(EXIT_FAILURE);
            }
        }
    }
    SDL_RenderPresent(renderer);

}

void closeDisplay() {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
