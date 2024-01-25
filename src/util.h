#include <stdbool.h>
#include <stdint.h>

/* Get bits value of byte from most significant to least significant */
void get_bits_from_byte(uint8_t byte, bool bits[8]);