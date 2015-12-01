#ifndef MIPS_CPU_DECDR_H
#define MIPS_CPU_DECDR_H

#include "mips_cpu.h"
#include "mips_cpu_modules.h"

mips_error r_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
);
void split_rtype_word(
	uint32_t instruction, 
	unsigned *machineWord
);
mips_error execute_r(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
);
mips_error execute_r_branch(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset
);
mips_error check_rword_validity(
	instr command, 
	unsigned* machineWord
);

mips_error i_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
);
void split_itype_word(
	uint32_t instruction,
	unsigned *machineWord
);
mips_error execute_i(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
);

mips_error execute_i_branch(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset, 
	bool link = false
);
mips_error execute_i_load(
	instr type, 
	mips_cpu_h state,
	unsigned *machineWord
);

mips_error execute_i_loadword_lf(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
);

mips_error execute_i_store(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
);

mips_error j_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
);
void split_jtype_word(
	uint32_t instruction, 
	unsigned *machineWord
);

mips_error execute_j(
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset, 
	bool link = false
);

mips_error get_source_registers(
	mips_cpu_h state, 
	uint32_t *valA, 
	unsigned *machineWord, 
	uint32_t *valB = NULL
);

#endif