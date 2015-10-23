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
void to_big_endian(uint32_t data, uint8_t *dataOutBuffer);
bool overflow_check(uint32_t A, uint32_t B);
void sign_extend_16_32(uint32_t *val);
mips_error decoder_module(mips_cpu_h state, uint32_t instruction);

enum instr
{
	//r-type
	add = 0x20,
	addu = 0x21,
	sub = 0x22, 
	subu = 0x23,
	bitwiseand = 0x24, 
	bitwiseor = 0x25,
	bitwisexor = 0x26, 
	//i-type
	bltz = 0,
	bgez = 0x1, 
	beq = 0x4,
	bne = 0x5,
	blez = 0x6,
	bgtz = 0x7,
	addi = 0x8,
	addiu = 0x9, 
	andi = 0xc,
	ori = 0xd,
	xori = 0xe,
	bltzal = 0x10, 
	bgezal = 0x11,
	lw = 0x23,
	sw = 0x2b,
};

mips_error alu_module(instr type, uint32_t A, uint32_t B, uint32_t *output);

#endif