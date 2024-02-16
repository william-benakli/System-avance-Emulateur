#include "cdp1802.h"
#include "cdp1802struct.h"
#include "instructions.h"

#include <stdbool.h>
#include <stdint.h>

void fetch(cdp1802_t *cpu) {
    cpu->register_i = cpu->memory[cpu->register_p] >> 4;
    cpu->register_n = cpu->memory[cpu->register_p] & 0xF;
}

void execute(cdp1802_t *cpu) {
    switch (cpu->register_i) {
    case 0: switch (cpu->register_n) {
        case 0: IDLE(cpu); break;
        default: LOAD_VIA_N(cpu); break;
        }
    case 1: INCREMENT_REG_N(cpu); break;
    case 2: DECREMENT_REG_N(cpu); break;
    case 3: switch (cpu->register_n) {
        case 0: UNCONDITIONAL_BRANCH(cpu); break;
        case 1: SHORT_BRANCH_IF_Q_EQUAL_ONE(cpu); break;
        case 2: SHORT_BRANCH_IF_D_EQUAL_ZERO(cpu); break;
        case 3: SHORT_BRANCH_IF_DF_EQUAL_ONE(cpu); break;
        case 4: SHORT_BRANCH_IF_EF1_EQUAL_ONE(cpu); break;
        case 5: SHORT_BRANCH_IF_EF2_EQUAL_ONE(cpu); break;
        case 6: SHORT_BRANCH_IF_EF3_EQUAL_ONE(cpu); break;
        case 7: SHORT_BRANCH_IF_EF4_EQUAL_ONE(cpu); break;
        case 8: NO_SHORT_BRANCH(cpu); break;
        case 9: SHORT_BRANCH_IF_Q_EQUAL_ZERO(cpu); break;
        case 0xA: SHORT_BRANCH_IF_D_NOT_ZERO(cpu); break;
        case 0xB: SHORT_BRANCH_IF_DF_EQUAL_ZERO(cpu); break;
        case 0xC: SHORT_BRANCH_IF_EF1_EQUAL_ZERO(cpu); break;
        case 0xD: SHORT_BRANCH_IF_EF2_EQUAL_ZERO(cpu); break;
        case 0xE: SHORT_BRANCH_IF_EF3_EQUAL_ZERO(cpu); break;
        case 0xF: SHORT_BRANCH_IF_EF4_EQUAL_ZERO(cpu); break;
        default: UNKNOWN_INSTRUCTION(cpu); break;
        } break;
    case 4: LOAD_ADVANCE(cpu); break;
    case 5: STORE_VIA_N(cpu); break;
    case 6: switch (cpu->register_n) {
        case 0: INCREMENT_REG_X(cpu); break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: OUTPUT(cpu); break;
        case 9: case 0xA: case 0xB: case 0xC: case 0xD: case 0xE: case 0xF: INPUT(cpu); break;
        default: UNKNOWN_INSTRUCTION(cpu); break;
        }
    case 7: switch (cpu->register_n) {
        case 0: RETURN(cpu); break;
        case 1: DISABLE(cpu); break;
        case 2: LOAD_VIA_X_AND_ADVANCE(cpu); break;
        case 3: STORE_VIA_X_AND_DECREMENT(cpu); break;
        case 4: ADD_WITH_CARRY(cpu); break;
        case 5: SUBTRACT_D_WITH_BORROW(cpu); break;
        case 6: SHIFT_RIGHT_WITH_CARRY(cpu); break;
        case 7: SUBSTRACT_MEMORY_WITH_BORROW(cpu); break;
        case 9: PUSH_X_P_TO_STACK(cpu); break;
        case 0xA: RESET_Q(cpu); break;
        case 0xB: SET_Q(cpu); break;
        case 0xC: ADD_WITH_CARRY_IMMEDIATE(cpu); break;
        case 0xD: SUBTRACT_D_WITH_BORROW_IMMEDIATE(cpu); break;
        case 0xE: SHIFT_LEFT_WITH_CARRY(cpu); break;
        case 0xF: SUBSTRACT_MEMORY_WITH_BORROW_IMMEDIATE(cpu); break;
        default: UNKNOWN_INSTRUCTION(cpu); break;
        } break;
    case 8: GET_LOW_REG_N(cpu); break;
    case 9: GET_HIGH_REG_N(cpu); break;
    case 0xA: PUT_LOW_REG_N(cpu); break;
    case 0xB: PUT_HIGH_REG_N(cpu); break;
    case 0xC: switch (cpu->register_n) {
        case 0: LONG_BRANCH(cpu); break;
        case 1: LONG_BRANCH_IF_Q_EQUAL_ONE(cpu); break;
        case 2: LONG_BRANCH_IF_D_EQUAL_ZERO(cpu); break;
        case 3: LONG_BRANCH_IF_DF_EQUAL_ONE(cpu); break;
        case 4: NO_OPERATION(cpu); break;
        case 5: LONG_SKIP_IF_Q_EQUAL_ZERO(cpu); break;
        case 6: LONG_SKIP_IF_D_NOT_ZERO(cpu); break;
        case 7: LONG_SKIP_IF_DF_EQUAL_ZERO(cpu); break;
        case 8: NO_LONG_BRANCH(cpu); break;
        case 9: LONG_BRANCH_IF_Q_EQUAL_ZERO(cpu); break;
        case 0xA: LONG_BRANCH_IF_D_NOT_ZERO(cpu); break;
        case 0xB: LONG_BRANCH_IF_DF_EQUAL_ZERO(cpu); break;
        case 0xC: LONG_SKIP_IF_IE_EQUAL_ONE(cpu); break;
        case 0xD: LONG_SKIP_IF_Q_EQUAL_ONE(cpu); break;
        case 0xE: LONG_SKIP_IF_D_EQUAL_ZERO(cpu); break;
        case 0xF: LONG_SKIP_IF_DF_EQUAL_ONE(cpu); break;
        default: UNKNOWN_INSTRUCTION(cpu); break;
        } break;
    case 0xD: SET_P(cpu); break;
    case 0xE: SET_X(cpu); break;
    case 0xF: switch (cpu->register_n) {
        case 0: LOAD_VIA_X(cpu); break;
        case 1: OR(cpu); break;
        case 2: AND(cpu); break;
        case 3: EXCLUSIVE_OR(cpu); break;
        case 4: ADD(cpu); break;
        case 5: SUBTRACT_D(cpu); break;
        case 6: SHIFT_RIGHT(cpu); break;
        case 7: SUBSTRACT_MEMORY(cpu); break;
        case 8: LOAD_IMMEDIATE(cpu); break;
        case 9: OR_IMMEDIATE(cpu); break;
        case 0xA: AND_IMMEDIATE(cpu); break;
        case 0xB: EXCLUSIVE_OR_IMMEDIATE(cpu); break;
        case 0xC: ADD_IMMEDIATE(cpu); break;
        case 0xD: SUBTRACT_D_IMMEDIATE(cpu); break;
        case 0xE: SHIFT_LEFT(cpu); break;
        case 0xF: SUBSTRACT_MEMORY_IMMEDIATE(cpu); break;
        default: UNKNOWN_INSTRUCTION(cpu); break;
        } break;
    default: UNKNOWN_INSTRUCTION(cpu); break;
    }
}

void cycle() {
    cdp1802_t cpu;
    while (true) {
        fetch(&cpu);
        execute(&cpu);
    }
}

int main() {
    return 0;
}

