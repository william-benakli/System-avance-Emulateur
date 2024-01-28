#include "display.h"

#include <stdio.h>

#define WINDOW_NAME "CHIP-8 Emulator"

#define SCALING 4

#define PRIMARY_COLOR 255, 255, 255, 255 
#define SECONDARY_COLOR 0, 0, 0, 255

SDL_Window *window;
SDL_Renderer *renderer;

void initializeDisplay(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

    window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALING, DISPLAY_HEIGHT * SCALING, SDL_WINDOW_SHOWN);
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

void handleInput() {
    SDL_Event event;
    if (SDL_PollEvent(&event) == 0) {
        return;
    }
    if (event.type == SDL_QUIT) {
        closeDisplay();
        exit(EXIT_SUCCESS);
    }
}

void refreshFrame() {
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT * SCALING];
    get_display(display);
    
    if (SDL_SetRenderDrawColor(renderer, SECONDARY_COLOR) != 0) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        closeDisplay();
        exit(EXIT_FAILURE);
    }
    SDL_RenderClear(renderer);
    if (SDL_SetRenderDrawColor(renderer, PRIMARY_COLOR) != 0) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        closeDisplay();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        if (display[i]) {
            SDL_Rect rect = {i % DISPLAY_WIDTH * SCALING, i / DISPLAY_WIDTH * SCALING, SCALING, SCALING};
            if (SDL_RenderFillRect(renderer, &rect) == -1) {
                fprintf(stderr, "Erreur SDL_RenderFillRect : %s", SDL_GetError());
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
