#include "libretro.h"
#include "chip8.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

chip8_t chip8;

retro_environment_t environ_cb;
retro_video_refresh_t video_cb;
retro_audio_sample_t audio_cb;
retro_audio_sample_batch_t audio_batch_cb;
retro_input_poll_t input_poll_cb;
retro_input_state_t input_state_cb;

struct retro_log_callback log_cb;

// static void keyboard_callback(bool down, unsigned keycode, uint32_t character, uint16_t mod);

RETRO_API void retro_set_environment(retro_environment_t cb) {
    environ_cb = cb;
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
    cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);
    cb(RETRO_ENVIRONMENT_GET_SAVESTATE_CONTEXT, NULL);
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t cb) {
    video_cb = cb;
}

RETRO_API void retro_set_audio_sample(retro_audio_sample_t cb) {
    audio_cb = cb;
}

RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
    audio_batch_cb = cb;
}

RETRO_API void retro_set_input_poll(retro_input_poll_t cb) {
    input_poll_cb = cb;
}

RETRO_API void retro_set_input_state(retro_input_state_t cb) {
    input_state_cb = cb;
}

RETRO_API void retro_init(void) {
    initialize_chip8(&chip8);
}

RETRO_API void retro_deinit(void) {
}

RETRO_API unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

RETRO_API void retro_get_system_info(struct retro_system_info *info) {
    // info->block_extract = false;
    info->library_name = "Chip8";
    info->library_version = "1.0";
    info->need_fullpath = true;
    info->valid_extensions = "ch8|rom";
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
    enum retro_pixel_format pixel_format = RETRO_PIXEL_FORMAT_RGB565;
    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
    
    info->geometry.base_width = DISPLAY_WIDTH;
    info->geometry.base_height = DISPLAY_HEIGHT;
    info->geometry.max_width = DISPLAY_WIDTH;
    info->geometry.max_height = DISPLAY_HEIGHT;
    info->geometry.aspect_ratio = 0.0;

    info->timing.fps = FRAMERATE;
    info->timing.sample_rate = 44100.0;
}

RETRO_API void retro_set_controller_port_device(unsigned port, unsigned device) {
    static struct retro_input_descriptor empty_input_descriptor[] = { { 0 } };
    struct retro_input_descriptor descriptions[2+1] = {0}; /* set final record to nulls */
    struct retro_input_descriptor *needle = &descriptions[0];

    log_cb.log(RETRO_LOG_INFO, "[CHIP-8] Blanking existing controller descriptions.\n", device, port);
    environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, empty_input_descriptor); /* is this necessary? it was in the sample code */

    log_cb.log(RETRO_LOG_INFO, "[CHIP-8] Plugging device %u into port %u.\n", device, port);    switch (device) {
        case RETRO_DEVICE_JOYPAD:
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_UP; needle->description = "0"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_DOWN; needle->description = "1"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_LEFT; needle->description = "2"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_RIGHT; needle->description = "3"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_A; needle->description = "4"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_B; needle->description = "5"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_X; needle->description = "6"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_Y; needle->description = "7"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_L; needle->description = "8"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_R; needle->description = "9"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_SELECT; needle->description = "A"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_START; needle->description = "B"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_L2; needle->description = "C"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_R2; needle->description = "D"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_L3; needle->description = "E"; needle++;
            needle->port = port; needle->device = device; needle->index = 0; needle->id = RETRO_DEVICE_ID_JOYPAD_R3; needle->description = "F"; needle++;
            break;
        default: log_cb.log(RETRO_LOG_ERROR, "[CHIP-8] Invalid device type: %u\n", device);
    }

    /* construct final zeroed record */
    needle->port = 0;  needle->device = 0;  needle->index = 0;
    needle->id = 0;    needle->description = NULL;
    environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, descriptions);
}

RETRO_API void retro_reset(void) {
}

RETRO_API void retro_run(void) {
    log_cb.log(RETRO_LOG_INFO, "[CHIP-8] Running frame.\n");
    // input
    input_poll_cb();
    chip8.pressed_keys[0x0] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP);
    chip8.pressed_keys[0x1] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN);
    chip8.pressed_keys[0x2] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
    chip8.pressed_keys[0x3] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT);
    chip8.pressed_keys[0x4] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
    chip8.pressed_keys[0x5] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
    chip8.pressed_keys[0x6] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X);
    chip8.pressed_keys[0x7] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y);
    chip8.pressed_keys[0x8] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
    chip8.pressed_keys[0x9] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
    chip8.pressed_keys[0xA] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT);
    chip8.pressed_keys[0xB] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
    chip8.pressed_keys[0xC] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2);
    chip8.pressed_keys[0xD] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2);
    chip8.pressed_keys[0xE] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);
    chip8.pressed_keys[0xF] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3) || input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3);

    // update chip 8
    update_timers(&chip8);
    run(&chip8, 1. / FRAMERATE * chip8.instructions_per_second);

    // graphics
    uint16_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        buffer[i] = chip8.display[i] ? 0xFFFF : 0x0000;
    }
    video_cb(buffer, DISPLAY_WIDTH, DISPLAY_HEIGHT, sizeof(uint16_t) * DISPLAY_WIDTH);
}

RETRO_API size_t retro_serialize_size(void) {
    return sizeof(chip8_t);
}

RETRO_API bool retro_serialize(void *data, size_t size) {
    if (size < sizeof(chip8_t)) {
        return false;
    }
    chip8_t *state = data;
    *state = chip8;
    return true;
}

RETRO_API bool retro_unserialize(const void *data, size_t size) {
    if (size < sizeof(chip8_t)) {
        return false;
    }
    const chip8_t *state = data;
    chip8 = *state;
    return true;
}

RETRO_API void retro_cheat_reset(void) {
}

RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char *code) {
}

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
    if (game == NULL) {
        return false;
    }
    // struct retro_keyboard_callback callback = {
    //     .callback = keyboard_callback,
    // };
    // environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &callback);
    // {
        /* data */
    // };
    
    return load_rom(&chip8, game->path);
}

RETRO_API bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) {
    return false;
}

RETRO_API void retro_unload_game(void) {
    unload_rom(&chip8);
}

RETRO_API unsigned retro_get_region(void) {
    return RETRO_REGION_NTSC;
}

RETRO_API void *retro_get_memory_data(unsigned id) {
    switch (id) {
    case RETRO_MEMORY_SYSTEM_RAM:
        return chip8.memory;
    default:
        return NULL;
    }
}

RETRO_API size_t retro_get_memory_size(unsigned id) {
    switch (id) {
    case RETRO_MEMORY_SYSTEM_RAM:
        return MEMORY_SIZE;
    default:
        return 0;
    }
}

/* static void keyboard_callback(bool down, unsigned keycode, uint32_t character, uint16_t mod) {
    uint8_t index;
    switch (keycode) {
    case RETROK_1: index = 0x1; break;
    case RETROK_2: index = 0x2; break;
    case RETROK_3: index = 0x3; break;
    case RETROK_4: index = 0xC; break;
    case RETROK_a: index = 0x4; break;
    case RETROK_z: index = 0x5; break;
    case RETROK_e: index = 0x6; break;
    case RETROK_r: index = 0xD; break;
    case RETROK_q: index = 0x7; break;
    case RETROK_s: index = 0x8; break;
    case RETROK_d: index = 0x9; break;
    case RETROK_f: index = 0xE; break;
    case RETROK_w: index = 0xA; break;
    case RETROK_x: index = 0x0; break;
    case RETROK_c: index = 0xB; break;
    case RETROK_v: index = 0xF; break;
    default: return;
    }
    chip8.pressed_keys[index] = down;
} */
