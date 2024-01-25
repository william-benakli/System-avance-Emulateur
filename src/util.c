#include "util.h"

void get_bits_from_byte(uint8_t byte, bool bits[8]) {
    for (int i = 0; i < 8; i++) {
        bits[i] = (byte >> (7 - i)) & 1;
    }
}
