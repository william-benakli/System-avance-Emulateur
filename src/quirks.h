#ifndef QUIRKS_H
#define QUIRKS_H

#define COSMAC_VIP 1 // FX0A: On the original COSMAC VIP, the key was only registered when it was pressed and then released
#define CHIP_48 2 // Starting with CHIP-48 and SUPER-CHIP, it was (probably unintentionally) changed to work as BXNN: It will jump to the address XNN, plus the value in the register VX. So the instruction B220 will jump to address 220 plus the value in the register V2.
#define SUPER_CHIP 3 // Starting with CHIP-48 and SUPER-CHIP, it was (probably unintentionally) changed to work as BXNN: It will jump to the address XNN, plus the value in the register VX. So the instruction B220 will jump to address 220 plus the value in the register V2.
#define AMIGA 4 // apparently the CHIP-8 interpreter for Amiga behaved this way. At least one known game, Spacefight 2091!, relies on this behavior. I don’t know of any games that rely on this not happening, so perhaps it’s safe to do it like the Amiga interpreter did.

typedef int machine_type_t;

int get_cycle_speed(machine_type_t machine_type);

#endif
