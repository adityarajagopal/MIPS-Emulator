#include "mips.h"

struct mips_cpu_impl
{
	uint32_t reg[32];
	uint32_t pc;

	mips_mem_h ram;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
	mips_cpu_h cpu = new mips_cpu_impl; 
	cpu->pc = 0;
	
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

	for(int i=0; i<32; i++)
	{
		state->reg[i] = 0;
	}
	//assume success for now. need to consider errors as defined in mips_core.h
	//create new function which handles exceptions, traps and memory management - MIPS-1 co-processor simulation
	return mips_Success;
}

mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
)
{
	if(state == 0)
	{
		return mips_ErrorInvalidHandle;
	}

	*value = state->reg[index];

	//assume success, i.e index is within range
	return mips_Success;
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

	state->reg[index] = value; 

	// assume index is within range
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

	//assume success
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

	//assume success
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
		return read;

	uint8_t *instruction = to_big_endian(dataInBuffer);

}

uint8_t *to_big_endian(uint8_t fetchedAdress)
{
	
}


