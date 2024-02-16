#include "instructions.h"

void INCREMENT_REG_N(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_n]++;
}

void DECREMENT_REG_N(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_n]--;
}

void INCREMENT_REG_X(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_x]++;
}

void GET_LOW_REG_N(cdp1802_t *cpu) {
    cpu->register_d = cpu->stratch_pad_registers[cpu->register_n] & 0xFF;
}

void PUT_LOW_REG_N(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_n] = (cpu->stratch_pad_registers[cpu->register_n] & 0xFF00) | cpu->register_d;
}

void GET_HIGH_REG_N(cdp1802_t *cpu) {
    cpu->register_d = cpu->stratch_pad_registers[cpu->register_n] >> 8;
}

void PUT_HIGH_REG_N(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_n] = (cpu->register_d << 8) | (cpu->stratch_pad_registers[cpu->register_n] & 0x00FF);
}

void LOAD_VIA_N(cdp1802_t *cpu) {
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_n]];
}

void LOAD_ADVANCE(cdp1802_t *cpu) {
    LOAD_VIA_N(cpu);
    INCREMENT_REG_N(cpu);    
}

void LOAD_VIA_X(cdp1802_t *cpu) {
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void LOAD_VIA_X_AND_ADVANCE(cdp1802_t *cpu) {
    LOAD_VIA_X(cpu);
    INCREMENT_REG_X(cpu);
}

void LOAD_IMMEDIATE(cdp1802_t *cpu) {
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void STORE_VIA_N(cdp1802_t *cpu) {
    cpu->memory[cpu->stratch_pad_registers[cpu->register_n]] = cpu->register_d;
}

void STORE_VIA_X_AND_DECREMENT(cdp1802_t *cpu) {
    cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] = cpu->register_d;
    cpu->stratch_pad_registers[cpu->register_x]--;
}

void OR(cdp1802_t *cpu) {
    cpu->register_d |= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void OR_IMMEDIATE(cdp1802_t *cpu) {
    cpu->register_d |= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void EXCLUSIVE_OR(cdp1802_t *cpu) {
    cpu->register_d ^= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void EXCLUSIVE_OR_IMMEDIATE(cdp1802_t *cpu) {
    cpu->register_d ^= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void AND(cdp1802_t *cpu) {
    cpu->register_d &= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void AND_IMMEDIATE(cdp1802_t *cpu) {
    cpu->register_d &= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SHIFT_RIGHT(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d & 1;
    cpu->register_d >>= 1;
}

void SHIFT_RIGHT_WITH_CARRY(cdp1802_t *cpu) {
    bool carry_flag = cpu->data_flag;
    SHIFT_RIGHT(cpu);
    cpu->register_d |= carry_flag << 7;
}

void SHIFT_LEFT(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d >> 7;
    cpu->register_d <<= 1;
}

void SHIFT_LEFT_WITH_CARRY(cdp1802_t *cpu) {
    bool carry_flag = cpu->data_flag;
    SHIFT_LEFT(cpu);
    cpu->register_d |= carry_flag;
}

void ADD(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d + cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] > 0xFF;
    cpu->register_d += cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void ADD_IMMEDIATE(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d + cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] > 0xFF;
    cpu->register_d += cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void ADD_WITH_CARRY(cdp1802_t *cpu) {
    bool carry_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] + cpu->register_d + cpu->data_flag > 0xFF;
    cpu->register_d += cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] + cpu->data_flag;
    cpu->data_flag = carry_flag;
}

void ADD_WITH_CARRY_IMMEDIATE(cdp1802_t *cpu) {
    bool carry_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] + cpu->register_d + cpu->data_flag > 0xFF;
    cpu->register_d += cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] + cpu->data_flag;
    cpu->data_flag = carry_flag;
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SUBTRACT_D(cdp1802_t *cpu) {
    cpu->data_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] >= cpu->register_d;
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] - cpu->register_d;
}

void SUBTRACT_D_IMMEDIATE(cdp1802_t *cpu) {
    cpu->data_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] >= cpu->register_d;
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] - cpu->register_d;
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SUBTRACT_D_WITH_BORROW(cdp1802_t *cpu) {
    bool carry_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] - cpu->register_d - !cpu->data_flag >= 0;
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] - cpu->register_d - !cpu->data_flag;
    cpu->data_flag = carry_flag;
}

void SUBTRACT_D_WITH_BORROW_IMMEDIATE(cdp1802_t *cpu) {
    bool carry_flag = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] - cpu->register_d - !cpu->data_flag >= 0;
    cpu->register_d = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] - cpu->register_d - !cpu->data_flag;
    cpu->data_flag = carry_flag;
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SUBSTRACT_MEMORY(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d >= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
    cpu->register_d -= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]];
}

void SUBSTRACT_MEMORY_IMMEDIATE(cdp1802_t *cpu) {
    cpu->data_flag = cpu->register_d >= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->register_d -= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]];
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SUBSTRACT_MEMORY_WITH_BORROW(cdp1802_t *cpu) {
    bool carry_flag = cpu->register_d - cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] - !cpu->data_flag >= 0;
    cpu->register_d -= cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] - !cpu->data_flag;
    cpu->data_flag = carry_flag;
}

void SUBSTRACT_MEMORY_WITH_BORROW_IMMEDIATE(cdp1802_t *cpu) {
    bool carry_flag = cpu->register_d - cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] - !cpu->data_flag >= 0;
    cpu->register_d -= cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] - !cpu->data_flag;
    cpu->data_flag = carry_flag;
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void UNCONDITIONAL_BRANCH(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_p] = cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] << 8;
}

void NO_SHORT_BRANCH(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_p]++;
}

void SHORT_BRANCH_IF_D_EQUAL_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d == 0) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_D_NOT_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d != 0) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_DF_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->data_flag) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_DF_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->data_flag) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_Q_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->q) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_Q_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->q) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF1_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->ef1) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF1_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->ef1) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF2_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->ef2) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF2_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->ef2) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF3_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->ef3) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF3_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->ef3) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF4_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->ef4) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void SHORT_BRANCH_IF_EF4_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->ef4) {
        UNCONDITIONAL_BRANCH(cpu);
    } else {
        NO_SHORT_BRANCH(cpu);
    }
}

void LONG_BRANCH(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_p] = (cpu->memory[cpu->stratch_pad_registers[cpu->register_p]] << 8) | cpu->memory[cpu->stratch_pad_registers[cpu->register_p] + 1];
}

void NO_LONG_BRANCH(cdp1802_t *cpu) {
    cpu->stratch_pad_registers[cpu->register_p] += 2;
}

void LONG_BRANCH_IF_D_EQUAL_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d == 0) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_BRANCH_IF_D_NOT_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d != 0) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_BRANCH_IF_DF_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->data_flag) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_BRANCH_IF_DF_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->data_flag) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_BRANCH_IF_Q_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->q) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_BRANCH_IF_Q_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->q) {
        LONG_BRANCH(cpu);
    } else {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_D_EQUAL_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d == 0) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_D_NOT_ZERO(cdp1802_t *cpu) {
    if (cpu->register_d != 0) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_DF_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->data_flag) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_DF_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->data_flag) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_Q_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->q) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_Q_EQUAL_ZERO(cdp1802_t *cpu) {
    if (!cpu->q) {
        NO_LONG_BRANCH(cpu);
    }
}

void LONG_SKIP_IF_IE_EQUAL_ONE(cdp1802_t *cpu) {
    if (cpu->interrupt_enable) {
        NO_LONG_BRANCH(cpu);
    }
}

void IDLE(cdp1802_t *cpu) {

}

void NO_OPERATION(cdp1802_t *cpu) {

}

void SET_P(cdp1802_t *cpu) {
    cpu->register_p = cpu->register_n;
}

void SET_X(cdp1802_t *cpu) {
    cpu->register_x = cpu->register_n;
}

void SET_Q(cdp1802_t *cpu) {
    cpu->q = true;
}

void RESET_Q(cdp1802_t *cpu) {
    cpu->q = false;
}

void INTERRUPT_ACTION(cdp1802_t *cpu) {
    cpu->register_t = (cpu->register_x & 0xF0) | (cpu->register_p >> 4);
    cpu->register_p = 1;
    cpu->register_x = 2;
    cpu->interrupt_enable = false;
}

void SAVE(cdp1802_t *cpu) {
    cpu->memory[cpu->stratch_pad_registers[cpu->register_x]] = cpu->register_t;
}

void PUSH_X_P_TO_STACK(cdp1802_t *cpu) {

}

void RETURN(cdp1802_t *cpu) {

}

void DISABLE(cdp1802_t *cpu) {

}

void OUTPUT(cdp1802_t *cpu) {

}

void INPUT(cdp1802_t *cpu) {

}

void DMA_IN_ACTION(cdp1802_t *cpu) {

}

void DMA_OUT_ACTION(cdp1802_t *cpu) {

}

void UNKNOWN_INSTRUCTION(cdp1802_t *cpu) {

}
