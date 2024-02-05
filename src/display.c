#include "display.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_NAME "CHIP-8 Emulator"

#define FRAMERATE 60
#define SCALING 12

#define PRIMARY_COLOR 255, 255, 255, 255 
#define SECONDARY_COLOR 0, 0, 0, 255

SDL_Window* window;
SDL_Renderer* renderer;
SDL_AudioDeviceID audio_device;
Uint8* audio_buf; // global pointer to the audio buffer to be played
Uint32 audio_len; // remaining length of the sample we have to play
SDL_AudioFormat format;

void handle_inputs(bool keys[16]);
void handle_sound(uint16_t sound_delay);
void refresh_frame(bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT * SCALING]);
void my_audio_callback(void *userdata, Uint8 *stream, int len);

int initialize_display() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        close_display();
        return -1;
    }

    // video
    window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALING, DISPLAY_HEIGHT * SCALING, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        close_display();
        return -1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        close_display();
        return -1;
    }

    // audio
    SDL_AudioSpec specs;
    SDL_RWops* rw = SDL_RWFromFile("./sfx/beep.wav", "rb");
    if (rw == NULL) {
        fprintf(stderr, "Erreur SDL_RWFromFile : %s", SDL_GetError());
        close_display();
        return -1;
    }
    
    if (SDL_LoadWAV_RW(rw, 1, &specs, &audio_buf, &audio_len) == NULL) {
        fprintf(stderr, "Erreur SDL_LoadWAV : %s", SDL_GetError());
        close_display();
        return -1;
    }
    specs.callback = my_audio_callback;
    specs.userdata = NULL;
    format = specs.format;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &specs, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (audio_device == 0) {
        fprintf(stderr, "Erreur SDL_OpenAudio : %s", SDL_GetError());
        close_display();
        return -1;
    }
    return 0;
}

void update_display(long double delta) {
    static long double bufferTime;
    bufferTime += delta;
    if (bufferTime >= 1. / FRAMERATE) { // update every 1/60th of a second
        bufferTime -= 1. / FRAMERATE;
        static bool keys[16];
        handle_inputs(keys);
        set_pressed_keys(keys);
        handle_sound(get_sound_delay());
        bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT * SCALING];
        get_display(display);
        refresh_frame(display);
    }
}

void close_display() {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    if (audio_device > 0) {
        SDL_CloseAudioDevice(audio_device);
    }
    SDL_FreeWAV(audio_buf);
    SDL_Quit();
}

void handle_inputs(bool keys[16]) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            close_display();
            exit(EXIT_SUCCESS);
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            uint8_t key_index;
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_1: key_index = 0x1; break;
            case SDL_SCANCODE_2: key_index = 0x2; break;
            case SDL_SCANCODE_3: key_index = 0x3; break;
            case SDL_SCANCODE_4: key_index = 0xC; break;
            case SDL_SCANCODE_Q: key_index = 0x4; break;
            case SDL_SCANCODE_W: key_index = 0x5; break;
            case SDL_SCANCODE_E: key_index = 0x6; break;
            case SDL_SCANCODE_R: key_index = 0xD; break;
            case SDL_SCANCODE_A: key_index = 0x7; break;
            case SDL_SCANCODE_S: key_index = 0x8; break;
            case SDL_SCANCODE_D: key_index = 0x9; break;
            case SDL_SCANCODE_F: key_index = 0xE; break;
            case SDL_SCANCODE_Z: key_index = 0xA; break;
            case SDL_SCANCODE_X: key_index = 0x0; break;
            case SDL_SCANCODE_C: key_index = 0xB; break;
            case SDL_SCANCODE_V: key_index = 0xF; break;
            default: return;
            }
            keys[key_index] = event.type == SDL_KEYDOWN;
        }
    }
}

void handle_sound(uint16_t sound_delay) {
    SDL_PauseAudioDevice(audio_device, sound_delay > 0 ? 0 : 1);
}

void refresh_frame(bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT * SCALING]) {
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

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
    /* if (audio_len == 0) {
        return;
    } */
    
    SDL_MixAudioFormat(stream, audio_buf, format, audio_len, SDL_MIX_MAXVOLUME);

    /* audio_buf += len; */
}
