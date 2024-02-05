#include "quirks.h"

int get_cycle_speed(machine_type_t machine_type) {
    switch (machine_type) {
    case COSMAC_VIP:
        return 1760000;
    case CHIP_48:
    case SUPER_CHIP:
    case AMIGA:
        return 500;
    }
    return 0;
}
