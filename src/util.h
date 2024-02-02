#include <stdbool.h>
#include <stdint.h>

/* Get bits value of byte from most significant to least significant */
void get_bits_from_byte(uint8_t byte, bool bits[8]);

void get_bits_from_2bytes(uint16_t bytes, bool bits[16]);

void printGraphics(bool display[]);
