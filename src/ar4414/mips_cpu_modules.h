#ifndef MIPS_CPU_MOD_H
#define MIPS_CPU_MOD_H

struct mips_cpu_impl
{
	uint32_t reg[32];
	uint32_t pc;
	uint32_t npc;
	uint32_t hi;
	uint32_t lo;

	mips_mem_provider *ram;
};

uint32_t to_little_endian(uint8_t *fetchedAdress);

void to_big_endian(
	uint32_t data, 
	uint8_t *dataOutBuffer
);

bool overflow_check(
	uint32_t A, 
	uint32_t B
);

void sign_extend_16_32(uint32_t *val);

void sign_extend_8_32(uint32_t *val);

mips_error decoder_module(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
);

mips_error mips_cpu_get_HILO(
	mips_cpu_h state, 
	bool HI, 
	uint32_t* value
);

mips_error mips_cpu_set_HILO(
	mips_cpu_h state, 
	bool HI, 
	uint32_t value
);

enum instr
{
	//r-type
	sll = 0x0,
	srl = 0x2,
	sra = 0x3,
	sllv = 0x4,
	srlv = 0x6,
	srav = 0x7,
	jr = 0x8,
	jalr = 0x9,
	mfhi = 0x10,
	mthi = 0x11,
	mtlo = 0x13,
	mflo = 0x12,
	mult = 0x18,
	multu = 0x19,
	divide = 0x1a,
	divu = 0x1b,
	add = 0x20,
	addu = 0x21,
	sub = 0x22, 
	subu = 0x23,
	bitwiseand = 0x24, 
	bitwiseor = 0x25,
	bitwisexor = 0x26, 
	slt = 0x2a,
	sltu = 0x2b,
	//i-type
	bltz = 0x100,
	bgez = 0x101, 
	beq = 0x104,
	bne = 0x105,
	blez = 0x106,
	bgtz = 0x107,
	addi = 0x108,
	addiu = 0x109, 
	slti = 0x10a,
	sltiu = 0x10b,
	andi = 0x10c,
	ori = 0x10d,
	xori = 0x10e,
	lui = 0x10f,
	bltzal = 0x110, 
	bgezal = 0x111,
	lb = 0x120, 
	lh = 0x121,
	lwl = 0x122,
	lw = 0x123,
	lbu = 0x124,
	lhu = 0x125,
	lwr = 0x126,
	sb = 0x128,
	sh = 0x129,
	sw = 0x12b,
};

mips_error alu_module(
	instr type, 
	uint32_t A, 
	uint32_t B, 
	uint32_t *output
);

#endif