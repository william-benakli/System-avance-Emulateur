#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "cdp1802struct.h"

// Synonyms

#define RING_SHIFT_RIGHT(cpu) SHIFT_RIGHT_WITH_CARRY(cpu)
#define RING_SHIFT_LEFT(cpu) SHIFT_LEFT_WITH_CARRY(cpu)
#define SHORT_SKIP(cpu) NO_SHORT_BRANCH(cpu)
#define SHORT_BRANCH_IF_POS_OR_ZERO(cpu) SHORT_BRANCH_IF_DF_EQUAL_ONE(cpu)
#define SHORT_BRANCH_IF_EQUAL_OR_GREATER(cpu) SHORT_BRANCH_IF_DF_EQUAL_ONE(cpu)
#define SHORT_BRANCH_IF_MINUS(cpu) SHORT_BRANCH_IF_DF_EQUAL_ZERO(cpu)
#define SHORT_BRANCH_IF_LESS(cpu) SHORT_BRANCH_IF_DF_EQUAL_ZERO(cpu)
#define LONG_SKIP(cpu) NO_LONG_BRANCH(cpu)

// Mnemonics

#define INC(cpu) INCREMENT_REG_N(cpu)
#define DEC(cpu) DECREMENT_REG_N(cpu)
#define IRX(cpu) INCREMENT_REG_X(cpu)
#define GLO(cpu) GET_LOW_REG_N(cpu)
#define PLO(cpu) PUT_LOW_REG_N(cpu)
#define GHI(cpu) GET_HIGH_REG_N(cpu)
#define PHI(cpu) PUT_HIGH_REG_N(cpu)

#define LDN(cpu) LOAD_VIA_N(cpu)
#define LDA(cpu) LOAD_ADVANCE(cpu)
#define LDX(cpu) LOAD_VIA_X(cpu)
#define LDXA(cpu) LOAD_VIA_X_AND_ADVANCE(cpu)
#define LDI(cpu) LOAD_IMMEDIATE(cpu)
#define STR(cpu) STORE_VIA_N(cpu)
#define STXD(cpu) STORE_VIA_X_AND_DECREMENT(cpu)

// #define OR(cpu) OR(cpu)
#define ORI(cpu) OR_IMMEDIATE(cpu)
#define XOR(cpu) EXCLUSIVE_OR(cpu)
#define XRI(cpu) EXCLUSIVE_OR_IMMEDIATE(cpu)
// #define AND(cpu) AND(cpu)
#define ANI(cpu) AND_IMMEDIATE(cpu)
#define SHR(cpu) SHIFT_RIGHT(cpu)
#define SHRC(cpu) SHIFT_RIGHT_WITH_CARRY(cpu)
#define RSHR(cpu) RING_SHIFT_RIGHT(cpu)
#define SHL(cpu) SHIFT_LEFT(cpu)
#define SHLC(cpu) SHIFT_LEFT_WITH_CARRY(cpu)
#define RSHL(cpu) RING_SHIFT_LEFT(cpu)

// #define ADD(cpu) ADD(cpu)
#define ADI(cpu) ADD_IMMEDIATE(cpu)
#define ADC(cpu) ADD_WITH_CARRY(cpu)
#define ADCI(cpu) ADD_WITH_CARRY_IMMEDIATE(cpu)
#define SD(cpu) SUBTRACT_D(cpu)
#define SDI(cpu) SUBTRACT_D_IMMEDIATE(cpu)
#define SDB(cpu) SUBTRACT_D_WITH_BORROW(cpu)
#define SDBI(cpu) SUBTRACT_D_WITH_BORROW_IMMEDIATE(cpu)
#define SM(cpu) SUBSTRACT_MEMORY(cpu)
#define SMI(cpu) SUBSTRACT_MEMORY_IMMEDIATE(cpu)
#define SMB(cpu) SUBSTRACT_MEMORY_WITH_BORROW(cpu)
#define SMBI(cpu) SUBSTRACT_MEMORY_WITH_BORROW_IMMEDIATE(cpu)

#define BR(cpu) UNCONDITIONAL_BRANCH(cpu)
#define NBR(cpu) NO_SHORT_BRANCH(cpu)
#define SKP(cpu) SHORT_SKIP(cpu)
#define BZ(cpu) SHORT_BRANCH_IF_D_EQUAL_ZERO(cpu)
#define BNZ(cpu) SHORT_BRANCH_IF_D_NOT_ZERO(cpu)
#define BDF(cpu) SHORT_BRANCH_IF_DF_EQUAL_ONE(cpu)
#define BPZ(cpu) SHORT_BRANCH_IF_POS_OR_ZERO(cpu)
#define BGE(cpu) SHORT_BRANCH_IF_EQUAL_OR_GREATER(cpu)
#define BNF(cpu) SHORT_BRANCH_IF_DF_EQUAL_ZERO(cpu)
#define BM(cpu) SHORT_BRANCH_IF_MINUS(cpu)
#define BL(cpu) SHORT_BRANCH_IF_LESS(cpu)
#define BQ(cpu) SHORT_BRANCH_IF_Q_EQUAL_ONE(cpu)
#define BNQ(cpu) SHORT_BRANCH_IF_Q_EQUAL_ZERO(cpu)
#define B1(cpu) SHORT_BRANCH_IF_EF1_EQUAL_ONE(cpu)
#define BN1(cpu) SHORT_BRANCH_IF_EF1_EQUAL_ZERO(cpu)
#define B2(cpu) SHORT_BRANCH_IF_EF2_EQUAL_ONE(cpu)
#define BN2(cpu) SHORT_BRANCH_IF_EF2_EQUAL_ZERO(cpu)
#define B3(cpu) SHORT_BRANCH_IF_EF3_EQUAL_ONE(cpu)
#define BN3(cpu) SHORT_BRANCH_IF_EF3_EQUAL_ZERO(cpu)
#define B4(cpu) SHORT_BRANCH_IF_EF4_EQUAL_ONE(cpu)
#define BN4(cpu) SHORT_BRANCH_IF_EF4_EQUAL_ZERO(cpu)

#define LBR(cpu) LONG_BRANCH(cpu)
#define NLBR(cpu) NO_LONG_BRANCH(cpu)
#define LSKP(cpu) LONG_SKIP(cpu)
#define LBZ(cpu) LONG_BRANCH_IF_D_EQUAL_ZERO(cpu)
#define LBNZ(cpu) LONG_BRANCH_IF_D_NOT_ZERO(cpu)
#define LBDF(cpu) LONG_BRANCH_IF_DF_EQUAL_ONE(cpu)
#define LBNF(cpu) LONG_BRANCH_IF_DF_EQUAL_ZERO(cpu)
#define LBQ(cpu) LONG_BRANCH_IF_Q_EQUAL_ONE(cpu)
#define LBNQ(cpu) LONG_BRANCH_IF_Q_EQUAL_ZERO(cpu)

// #define SKP(cpu) SHORT_SKIP(cpu)
// #define LSKP(cpu) LONG_SKIP(cpu)
#define LSZ(cpu) LONG_SKIP_IF_D_EQUAL_ZERO(cpu)
#define LSNZ(cpu) LONG_SKIP_IF_D_NOT_ZERO(cpu)
#define LSDF(cpu) LONG_SKIP_IF_DF_EQUAL_ONE(cpu)
#define LSNF(cpu) LONG_SKIP_IF_DF_EQUAL_ZERO(cpu)
#define LSQ(cpu) LONG_SKIP_IF_Q_EQUAL_ONE(cpu)
#define LSNQ(cpu) LONG_SKIP_IF_Q_EQUAL_ZERO(cpu)
#define LSIE(cpu) LONG_SKIP_IF_IE_EQUAL_ONE(cpu)

#define IDL(cpu) IDLE(cpu)
#define NOP(cpu) NO_OPERATION(cpu)
#define SEP(cpu) SET_P(cpu)
#define SEX(cpu) SET_X(cpu)
#define SEQ(cpu) SET_Q(cpu)
#define REQ(cpu) RESET_Q(cpu)

#define SAV(cpu) SAVE(cpu)
#define MARK(cpu) PUSH_X_P_TO_STACK(cpu)
#define RET(cpu) RETURN(cpu)
#define DIS(cpu) DISABLE(cpu)

#define OUT(cpu) OUTPUT(cpu)
#define INP(cpu) INPUT(cpu)

// Register Operations

void INCREMENT_REG_N(cdp1802_t *cpu); // 1N
void DECREMENT_REG_N(cdp1802_t *cpu); // 2N
void INCREMENT_REG_X(cdp1802_t *cpu); // 60
void GET_LOW_REG_N(cdp1802_t *cpu); // 8N
void PUT_LOW_REG_N(cdp1802_t *cpu); // AN
void GET_HIGH_REG_N(cdp1802_t *cpu); // 9N
void PUT_HIGH_REG_N(cdp1802_t *cpu); // BN

// Memory Reference

void LOAD_VIA_N(cdp1802_t *cpu); // 0N
void LOAD_ADVANCE(cdp1802_t *cpu); // 4N
void LOAD_VIA_X(cdp1802_t *cpu); // F0
void LOAD_VIA_X_AND_ADVANCE(cdp1802_t *cpu); // 72
void LOAD_IMMEDIATE(cdp1802_t *cpu); // F8
void STORE_VIA_N(cdp1802_t *cpu); // 5N
void STORE_VIA_X_AND_DECREMENT(cdp1802_t *cpu); // 73

// Logic Operations 

void OR(cdp1802_t *cpu);
void OR_IMMEDIATE(cdp1802_t *cpu);
void EXCLUSIVE_OR(cdp1802_t *cpu);
void EXCLUSIVE_OR_IMMEDIATE(cdp1802_t *cpu);
void AND(cdp1802_t *cpu);
void AND_IMMEDIATE(cdp1802_t *cpu);
void SHIFT_RIGHT(cdp1802_t *cpu);
void SHIFT_RIGHT_WITH_CARRY(cdp1802_t *cpu);
void SHIFT_LEFT(cdp1802_t *cpu);
void SHIFT_LEFT_WITH_CARRY(cdp1802_t *cpu);

// Arithmetic Operations 

void ADD(cdp1802_t *cpu);
void ADD_IMMEDIATE(cdp1802_t *cpu);
void ADD_WITH_CARRY(cdp1802_t *cpu);
void ADD_WITH_CARRY_IMMEDIATE(cdp1802_t *cpu);
void SUBTRACT_D(cdp1802_t *cpu);
void SUBTRACT_D_IMMEDIATE(cdp1802_t *cpu);
void SUBTRACT_D_WITH_BORROW(cdp1802_t *cpu);
void SUBTRACT_D_WITH_BORROW_IMMEDIATE(cdp1802_t *cpu);
void SUBSTRACT_MEMORY(cdp1802_t *cpu);
void SUBSTRACT_MEMORY_IMMEDIATE(cdp1802_t *cpu);
void SUBSTRACT_MEMORY_WITH_BORROW(cdp1802_t *cpu);
void SUBSTRACT_MEMORY_WITH_BORROW_IMMEDIATE(cdp1802_t *cpu);

// Short-Branch Operations

void UNCONDITIONAL_BRANCH(cdp1802_t *cpu);
void NO_SHORT_BRANCH(cdp1802_t *cpu);
void SHORT_BRANCH_IF_D_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_D_NOT_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_DF_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_DF_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_Q_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_Q_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF1_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF1_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF2_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF2_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF3_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF3_EQUAL_ZERO(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF4_EQUAL_ONE(cdp1802_t *cpu);
void SHORT_BRANCH_IF_EF4_EQUAL_ZERO(cdp1802_t *cpu);

// Long-Branch Operations

void LONG_BRANCH(cdp1802_t *cpu);
void NO_LONG_BRANCH(cdp1802_t *cpu);
void LONG_BRANCH_IF_D_EQUAL_ZERO(cdp1802_t *cpu);
void LONG_BRANCH_IF_D_NOT_ZERO(cdp1802_t *cpu);
void LONG_BRANCH_IF_DF_EQUAL_ONE(cdp1802_t *cpu);
void LONG_BRANCH_IF_DF_EQUAL_ZERO(cdp1802_t *cpu);
void LONG_BRANCH_IF_Q_EQUAL_ONE(cdp1802_t *cpu);
void LONG_BRANCH_IF_Q_EQUAL_ZERO(cdp1802_t *cpu);

// Skip Instructions

// void SHORT_SKIP(cdp1802_t *cpu);
// void LONG_SKIP(cdp1802_t *cpu);
void LONG_SKIP_IF_D_EQUAL_ZERO(cdp1802_t *cpu);
void LONG_SKIP_IF_D_NOT_ZERO(cdp1802_t *cpu);
void LONG_SKIP_IF_DF_EQUAL_ONE(cdp1802_t *cpu);
void LONG_SKIP_IF_DF_EQUAL_ZERO(cdp1802_t *cpu);
void LONG_SKIP_IF_Q_EQUAL_ONE(cdp1802_t *cpu);
void LONG_SKIP_IF_Q_EQUAL_ZERO(cdp1802_t *cpu);
void LONG_SKIP_IF_IE_EQUAL_ONE(cdp1802_t *cpu);

// Control Instructions

void IDLE(cdp1802_t *cpu);
void NO_OPERATION(cdp1802_t *cpu);
void SET_P(cdp1802_t *cpu);
void SET_X(cdp1802_t *cpu);
void SET_Q(cdp1802_t *cpu);
void RESET_Q(cdp1802_t *cpu);

// Interrupt and Subroutine Handling

void INTERRUPT_ACTION(cdp1802_t *cpu);
void SAVE(cdp1802_t *cpu);
void PUSH_X_P_TO_STACK(cdp1802_t *cpu);
void RETURN(cdp1802_t *cpu);
void DISABLE(cdp1802_t *cpu);

// Input/Output Byte Transfer

void OUTPUT(cdp1802_t *cpu);
void INPUT(cdp1802_t *cpu);

// DMA Servicing

void DMA_IN_ACTION(cdp1802_t *cpu);
void DMA_OUT_ACTION(cdp1802_t *cpu);

void UNKNOWN_INSTRUCTION(cdp1802_t *cpu);

#endif
