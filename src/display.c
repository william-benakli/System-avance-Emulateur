#include "display.h"

#include <stdio.h>

#define WINDOW_NAME "CHIP-8 Emulator"

#define SCALING 4

#define PRIMARY_COLOR 255, 255, 255, 255 
#define SECONDARY_COLOR 0, 0, 0, 255

SDL_Window *window;
SDL_Renderer *renderer;

void initialize_display() {
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
        close_display();
        exit(EXIT_FAILURE);
    }
}

uint16_t handle_inputs() {
    uint16_t keys = 0;
    SDL_Event event;
    if (SDL_PollEvent(&event) == 0) {
        return 0;
    }
    if (event.type == SDL_QUIT) {
        close_display();
        exit(EXIT_SUCCESS);
    }
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        uint8_t key_index;
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_1:
                key_index = 0x1;
                break;
            case SDL_SCANCODE_2:
                key_index = 0x2;
                break;
            case SDL_SCANCODE_3:
                key_index = 0x3;
                break;
            case SDL_SCANCODE_4:
                key_index = 0xC;
                break;
            case SDL_SCANCODE_Q:
                key_index = 0x4;
                break;
            case SDL_SCANCODE_W:
                key_index = 0x5;
                break;
            case SDL_SCANCODE_E:
                key_index = 0x6;
                break;
            case SDL_SCANCODE_R:
                key_index = 0xD;
                break;
            case SDL_SCANCODE_A:
                key_index = 0x7;
                break;
            case SDL_SCANCODE_S:
                key_index = 0x8;
                break;
            case SDL_SCANCODE_D:
                key_index = 0x9;
                break;
            case SDL_SCANCODE_F:
                key_index = 0xE;
                break;
            case SDL_SCANCODE_Z:
                key_index = 0xA;
                break;
            case SDL_SCANCODE_X:
                key_index = 0x0;
                break;
            case SDL_SCANCODE_C:
                key_index = 0xB;
                break;
            case SDL_SCANCODE_V:
                key_index = 0xF;
                break;
            default:
                break;
        }
        keys |= (event.type == SDL_KEYDOWN ? 1 : 0) << key_index;
    }
    return keys;
}

void refresh_frame() {
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT * SCALING];
    get_display(display);
    
    if (SDL_SetRenderDrawColor(renderer, SECONDARY_COLOR) != 0) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        close_display();
        exit(EXIT_FAILURE);
    }
    SDL_RenderClear(renderer);
    if (SDL_SetRenderDrawColor(renderer, PRIMARY_COLOR) != 0) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        close_display();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        if (display[i]) {
            SDL_Rect rect = {i % DISPLAY_WIDTH * SCALING, i / DISPLAY_WIDTH * SCALING, SCALING, SCALING};
            if (SDL_RenderFillRect(renderer, &rect) == -1) {
                fprintf(stderr, "Erreur SDL_RenderFillRect : %s", SDL_GetError());
                close_display();
                exit(EXIT_FAILURE);
            }
        }
    }
    SDL_RenderPresent(renderer);

}

void close_display() {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
