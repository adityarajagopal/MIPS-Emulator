#include "mips.h"
#include "mips_cpu_decoder.h"
#include "mips_cpu_modules.h"
#include <iostream>

using namespace std;

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
	mips_cpu_h cpu = new mips_cpu_impl; 
	cpu->pc = 0;
	cpu->npc = 0;
	cpu->hi = 0;
	cpu->lo = 0;
	
	for(int i = 0; i<32; i++)
	{
		cpu->reg[i] = 0;
	}
	
	cpu->ram = mem;

	return cpu;
}

mips_error mips_cpu_reset(mips_cpu_h state)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	state->pc = 0;
	state->npc = 0;
	state->hi = 0;
	state->lo = 0;

	for(int i=0; i<32; i++)
	{
		state->reg[i] = 0;
	}

	return mips_Success;
}

mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
)
{
	if(state == 0)
		return mips_ErrorInvalidHandle;

	if(index <= 31)
	{
		*value = (state->reg[index]);
		return mips_Success;
	}

	else
		return mips_ExceptionAccessViolation;
}

mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	if(index == 0)
		return mips_Success;

	if(index <= 31)
	{
		state->reg[index] = value; 
		return mips_Success;
	}
	
	else
		return mips_ExceptionAccessViolation;
}

mips_error mips_cpu_get_HILO(
	mips_cpu_h state, 
	bool HI, 
	uint32_t* value
)
{
	if(state == 0)
		return mips_ErrorInvalidHandle;

	if(HI)
		*value = state->hi;
	else 
		*value = state->lo;

	return mips_Success;
}

mips_error mips_cpu_set_HILO(
	mips_cpu_h state, 
	bool HI, 
	uint32_t value
)
{
	if(state == 0)
		return mips_ErrorInvalidHandle;

	if(HI)
		state->hi = value;
	else 
		state->lo = value;

	return mips_Success;
}

mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	state->pc = pc;
	state->npc = pc+4;
	return mips_Success;
}

mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	*pc = state->pc;

	return mips_Success;
}

mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	//fetch from memory
	uint8_t dataInBuffer[4];
	mips_error read = mips_mem_read(state->ram, state->pc, 4, dataInBuffer);

	if(read != mips_Success) 
	{
		return read;
	}

	uint32_t instruction = to_little_endian(dataInBuffer);

	uint32_t offset = 0;
	mips_error execute = decoder_module(state, instruction, &offset);
	if(execute != mips_Success)
	{
		return execute;
	}

	execute = mips_cpu_set_pc(state, state->npc);
	state->npc += offset;

	return execute;
}

void mips_cpu_free(mips_cpu_h state)
{
	if(state)
		delete state;
}

mips_error mips_cpu_set_debug_level(
	mips_cpu_h state, 
	unsigned level, 
	FILE *dest
)
{	

	return mips_Success;
}

uint32_t to_little_endian(uint8_t *fetchedAdress)
{
	uint32_t bigEndianWord = 
	(
		(((uint32_t)fetchedAdress[3]) << 0) | 
		(((uint32_t)fetchedAdress[2]) << 8) | 
		(((uint32_t)fetchedAdress[1]) << 16) | 
		(((uint32_t)fetchedAdress[0]) << 24)
	);
	
	return bigEndianWord;
}

void to_big_endian(
	uint32_t data, 
	uint8_t *dataOutBuffer
)
{
	dataOutBuffer[0] = (data >> 24)&0xff;
	dataOutBuffer[1] = ((data << 8) >> 24)&0xff;
	dataOutBuffer[2] = ((data << 16) >> 24)&0xff;
	dataOutBuffer[3] = ((data << 24) >> 24)&0xff;
}

void sign_extend_16_32(uint32_t *val)
{
	if((*val << 16) & 0x80000000)
	{
		*val = 0xffff0000 | *val;
	}
}

void sign_extend_8_32(uint32_t *val)
{
	if((*val << 24) & 0x80000000)
	{
		*val = 0xffffff00 | *val;
	}
}

void sign_extend_32_64(long long* val)
{
	if((*val >> 31) & 0x1)
	{
		*val = *val | 0xffffffff00000000;
	}
}

mips_error decoder_module(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
)
{
	uint32_t opcode;
	mips_error execute;

	opcode = instruction >> 26;

	if(opcode == 0)
	{
		execute = r_type(state, instruction, offset);
	}

	else if ((opcode >> 1) == 0x1)
	{
		execute = j_type(state, instruction, offset);
	}

	else 
	{
		execute = i_type(state, instruction, offset);
	}

	return execute;
}

mips_error alu_module(
	instr type, 
	uint32_t A, 
	uint32_t B, 
	uint32_t *output
)
{
	if(type == addi)
		type = add;
	else if((type == addiu) || (type == lw) || (type == lb) || (type == lbu) || (type == lh) || (type == lhu) || (type == lwl) || (type == lwr) || (type == sw) || (type == sb) || (type == sh))
		type = addu;
	else if(type == andi)
		type = bitwiseand;
	else if(type == ori)
		type = bitwiseor;
	else if(type == xori)
		type = bitwisexor;
	else if(type == bltzal)
		type = bltz;
	else if (type == bgezal)
		type = bgez;
	else if(type == slti)
		type = slt;
	else if(type == sltiu)
		type = sltu;

	switch(type)
	{
		case add:
		{	
			output[0] = A + B;
			if(overflow_check(A,B))
				output[1] = 1;
			else
				output[1] = 0;
			return mips_Success;
		}

		case addu:
		{
			output[0] = A + B;
			output[1] = 0;
			return mips_Success;
		}

		case sub:
		{
			B = (~B) + 1;
			output[0] = A + B;
			if(overflow_check(A,B))
				output[1] = 1;
			else
				output[1] = 0;
			return mips_Success;
		}

		case subu:
		{
			B = (~B) + 1;
			output[0] = A + B;
			output[1] = 0;
			return mips_Success;
		}

		case bitwiseand:
		{
			output[0] = A & B;
			output[1] = 0;
			return mips_Success;
		}

		case bitwiseor:
		{
			output[0] = A | B;
			output[1] = 0;
			return mips_Success;
		}

		case bitwisexor:
		{
			output[0] = A ^ B;
			output[1] = 0;
			return mips_Success;
		}

		case beq:
		{
			if (A == B)
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case bne:
		{
			if(A != B)
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case bgtz:
		{
			if((A!=0) && !(A & 0x80000000))
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case blez:
		{
			if((A==0) || (A & 0x80000000))
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case bltz:
		{
			if(A & 0x80000000)
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case bgez:
		{
			if(!(A & 0x80000000))
				*output = 1;
			else 
				*output = 0;
			return mips_Success;
		}

		case sltu:
		{
			output[1] = 0;
			if(A < B)
				output[0] = 1;
			else 
				output[0] = 0;
			return mips_Success;
		}

		case slt: 
		{
			output[1] = 0;
			if((int) A < (int) B)
				output[0] = 1;
			else 
				output[0] = 0;
			return mips_Success;
		}

		case divide:
		{
			if(B == 0)
				return mips_Success;
			output[0] = ((int) A / (int) B);
			output[1] = ((int) A % (int) B);
			return mips_Success;
		}

		case divu:
		{	
			if(B == 0)
				return mips_Success;
			output[0] = A / B; 
			output[1] = A % B;
			return mips_Success;
		}

		case mult:
		{
			long long valA = (long long) A;
			long long valB = (long long) B;
			sign_extend_32_64(&valA);
			sign_extend_32_64(&valB);

			long long result = valA * valB;

			output[0] = result & 0xffffffff;
			output[1] = result >> 32;
			return mips_Success;
		}

		case multu:
		{
			unsigned long long valA = (unsigned long long) A;
			unsigned long long valB = (unsigned long long) B;

			unsigned long long result = valA * valB;

			output[0] = result & 0xffffffff;
			output[1] = result >> 32;
			return mips_Success;
		}

		default :
			return mips_ExceptionInvalidInstruction;
	}
}

bool overflow_check(
	uint32_t A, 
	uint32_t B
)
{
	uint32_t result = A + B;
	//if A and B are positive
	if(!(A & 0x80000000) && !(B & 0x80000000))
	{
		if(result & 0x80000000)
			return true;
		else 
			return false;
	}
	//if A and B are negative
	else if((A & 0x80000000) && (B & 0x80000000))
	{
		if(!(result & 0x80000000))
			return true;
		else 
			return false;
	}

	else 
		return false;
}


