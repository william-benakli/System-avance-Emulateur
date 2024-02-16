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
    bool no_rom = false;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
    cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);
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
    info->block_extract = false;
    info->library_name = "Chip8";
    info->library_version = "1.0";
    info->need_fullpath = true;
    info->valid_extensions = "ch8|rom";
    printf("\n[README ENFOIRE] Libretro info\n\n");
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
}

RETRO_API void retro_reset(void) {
}

RETRO_API void retro_run(void) {
    printf("\n[README ENFOIRE] Running\n\n");
    // input
    input_poll_cb();

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
    return 0;
}

RETRO_API bool retro_serialize(void *data, size_t size) {
    return false;
}

RETRO_API bool retro_unserialize(const void *data, size_t size) {
    return false;
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
    return load_rom(&chip8, game->path);
}

RETRO_API bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) {
    return false;
}

RETRO_API void retro_unload_game(void) {
    unload_rom(&chip8);
    printf("\n[README ENFOIRE] Jeu déchargé.\n\n");
}

RETRO_API unsigned retro_get_region(void) {
    return RETRO_REGION_NTSC;
}

RETRO_API void *retro_get_memory_data(unsigned id) {
    if (id == RETRO_MEMORY_SYSTEM_RAM) {
        return chip8.memory;
    }
    return NULL;
}

RETRO_API size_t retro_get_memory_size(unsigned id) {
    if (id == RETRO_MEMORY_SYSTEM_RAM) {
        return MEMORY_SIZE;
    }
    return 0;
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
