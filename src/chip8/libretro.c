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

static void keyboard_callback(bool down, unsigned keycode, uint32_t character, uint16_t mod);

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
    struct retro_input_descriptor desc[16];
    switch (device) {
        case RETRO_DEVICE_JOYPAD:
            desc[0].port = port; desc[0].device = device; desc[0].index = 0; desc[0].id = RETRO_DEVICE_ID_JOYPAD_UP; desc[0].description = "0";
            desc[1].port = port; desc[1].device = device; desc[1].index = 0; desc[1].id = RETRO_DEVICE_ID_JOYPAD_DOWN; desc[1].description = "1";
            desc[2].port = port; desc[2].device = device; desc[2].index = 0; desc[2].id = RETRO_DEVICE_ID_JOYPAD_LEFT; desc[2].description = "2";
            desc[3].port = port; desc[3].device = device; desc[3].index = 0; desc[3].id = RETRO_DEVICE_ID_JOYPAD_RIGHT; desc[3].description = "3";
            desc[4].port = port; desc[4].device = device; desc[4].index = 0; desc[4].id = RETRO_DEVICE_ID_JOYPAD_A; desc[4].description = "4";
            desc[5].port = port; desc[5].device = device; desc[5].index = 0; desc[5].id = RETRO_DEVICE_ID_JOYPAD_B; desc[5].description = "5";
            desc[6].port = port; desc[6].device = device; desc[6].index = 0; desc[6].id = RETRO_DEVICE_ID_JOYPAD_X; desc[6].description = "6";
            desc[7].port = port; desc[7].device = device; desc[7].index = 0; desc[7].id = RETRO_DEVICE_ID_JOYPAD_Y; desc[7].description = "7";
            desc[8].port = port; desc[8].device = device; desc[8].index = 0; desc[8].id = RETRO_DEVICE_ID_JOYPAD_L; desc[8].description = "8";
            desc[9].port = port; desc[9].device = device; desc[9].index = 0; desc[9].id = RETRO_DEVICE_ID_JOYPAD_R; desc[9].description = "9";
            desc[10].port = port; desc[10].device = device; desc[10].index = 0; desc[10].id = RETRO_DEVICE_ID_JOYPAD_SELECT; desc[10].description = "A";
            desc[11].port = port; desc[11].device = device; desc[11].index = 0; desc[11].id = RETRO_DEVICE_ID_JOYPAD_START; desc[11].description = "B";
            desc[12].port = port; desc[12].device = device; desc[12].index = 0; desc[12].id = RETRO_DEVICE_ID_JOYPAD_L2; desc[12].description = "C";
            desc[13].port = port; desc[13].device = device; desc[13].index = 0; desc[13].id = RETRO_DEVICE_ID_JOYPAD_R2; desc[13].description = "D";
            desc[14].port = port; desc[14].device = device; desc[14].index = 0; desc[14].id = RETRO_DEVICE_ID_JOYPAD_L3; desc[14].description = "E";
            desc[15].port = port; desc[15].device = device; desc[15].index = 0; desc[15].id = RETRO_DEVICE_ID_JOYPAD_R3; desc[15].description = "F";
            break;
        default: log_cb.log(RETRO_LOG_ERROR, "Invalid device type: %u\n", device);
    }
    environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, &desc);
}

RETRO_API void retro_reset(void) {
}

RETRO_API void retro_run(void) {
    // input
    input_poll_cb();
    chip8.pressed_keys[0x4] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X);
    chip8.pressed_keys[0x5] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
    chip8.pressed_keys[0x6] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT);
    chip8.pressed_keys[0x7] = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN);

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
    log_cb.log(RETRO_LOG_INFO, "retro_serialize_size\n");
    return sizeof(chip8_t);
}

RETRO_API bool retro_serialize(void *data, size_t size) {
    log_cb.log(RETRO_LOG_INFO, "retro_serialize\n");
    if (size < sizeof(chip8_t)) {
        return false;
    }
    chip8_t *state = data;
    *state = chip8;
    return true;
}

RETRO_API bool retro_unserialize(const void *data, size_t size) {
    log_cb.log(RETRO_LOG_INFO, "retro_unserialize\n");
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
    struct retro_keyboard_callback callback = {
        .callback = keyboard_callback,
    };
    environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &callback);
    {
        /* data */
    };
    
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

static void keyboard_callback(bool down, unsigned keycode, uint32_t character, uint16_t mod) {
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
}
