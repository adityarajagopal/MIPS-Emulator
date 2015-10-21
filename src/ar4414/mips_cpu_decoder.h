#ifndef MIPS_CPU_DECDR_H
#define MIPS_CPU_DECDR_H

#include "mips_cpu.h"

mips_error r_type(mips_cpu_h state, uint32_t instruction);
mips_error r_mword_split(mips_cpu_h state, uint32_t instruction, uint32_t *valA, uint32_t *valB, unsigned *destReg);

mips_error ADDU(mips_cpu_h state, uint32_t instruction);
mips_error ADD(mips_cpu_h state, uint32_t instruction);
mips_error AND(mips_cpu_h state, uint32_t instruction);
mips_error OR(mips_cpu_h state, uint32_t instruction);
mips_error XOR(mips_cpu_h state, uint32_t instruction);

mips_error i_type(mips_cpu_h state, uint32_t instruction);

mips_error j_type(mips_cpu_h state, uint32_t instruction);

enum instructions
{
	add = 0x20,
	addu = 0x21,
	bitwiseand = 0x24, 
	bitwiseor = 0x25,
	bitwisexor = 0x26
};

#endif