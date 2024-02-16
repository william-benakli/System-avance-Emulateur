#ifndef CDP1802STRUCT_H
#define CDP1802STRUCT_H

#define MAX_MEMORY_SIZE 65536 // direct memory addressing up to 65,536 bytes
#define REGISTER_SIZE 16 // 16 X 16 matrix of registers for use as multiple program counters, data pointers, or data registers

#include <stdbool.h>
#include <stdint.h>

typedef struct cdp1802 {
    uint8_t register_n; // N
    uint8_t register_p; // P
    uint8_t register_x; // X
    uint8_t register_i; // I
    uint8_t register_d; // D
    uint8_t register_t; // T
    bool data_flag; // DF
    bool interrupt_enable; // IE
    uint16_t stratch_pad_registers[REGISTER_SIZE]; // R
    uint8_t memory[MAX_MEMORY_SIZE]; // M

    // inputs
    bool ef1; // EF1
    bool ef2; // EF2
    bool ef3; // EF3
    bool ef4; // EF4

    // output
    bool q; // Q
} cdp1802_t;

#endif
