#include "mips.h"
#include "mips_cpu_decoder.h"
#include <iostream>
using namespace std;

mips_error r_type(mips_cpu_h state, uint32_t instruction)
{
	uint32_t fnCode;  
	instructions opcode;

	fnCode = (instruction << 26) >> 26;
	opcode = (instructions) fnCode;

	switch(opcode)
	{
		//ADDU [ADD Unsigned]
		case addu:
		{
			cout << "addu" << endl;
			mips_error err = ADDU(state, instruction);
			return err;
		}

		case add:
		{
			cout << "add" << endl;
			mips_error err = ADD(state, instruction);
			return err;
		}

		case bitwiseand:
		{
			cout << "and" << endl;
			mips_error err = AND(state, instruction);
			return err;
		}

		case bitwiseor:
		{
			cout << "or" << endl;
			mips_error err = OR(state, instruction);
			return err;
		}

		case bitwisexor:
		{
			cout <<"xor"<< endl;
			mips_error err = XOR(state, instruction);
			return err;
		}

		default: 
		{
			return mips_ExceptionInvalidInstruction;
		}
	}
}

mips_error ADDU(mips_cpu_h state, uint32_t instruction)
{
	unsigned registerA = (instruction << 6) >> 27;
	unsigned registerB = (instruction << 11) >> 27;
	unsigned destReg = (instruction << 16) >> 27;
	
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];

	mips_error err = mips_cpu_get_register(state, registerA, &valA);		
	if(err != mips_Success)
		return err;
	
	err = mips_cpu_get_register(state, registerB, &valB);	
	if(err != mips_Success)
		return err;

	alu_select type = sum;
	alu_module(type, valA, valB, aluOP);

	err = mips_cpu_set_register(state, destReg, aluOP[0]);

	if(err == mips_Success)
	{
		//increment PC
		state->pc = state->npc;
		state->npc += 4;		
	}

	return err;
}

mips_error ADD(mips_cpu_h state, uint32_t instruction)
{
	unsigned registerA = (instruction << 6) >> 27;
	unsigned registerB = (instruction << 11) >> 27;
	unsigned destReg = (instruction << 16) >> 27;
	
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];

	mips_error err = mips_cpu_get_register(state, registerA, &valA);		
	if(err != mips_Success)
		return err;
	
	err = mips_cpu_get_register(state, registerB, &valB);	
	if(err != mips_Success)
		return err;
	
	alu_select type = sum;
	alu_module(type, valA, valB, aluOP);

	if(aluOP[1])
		return mips_ExceptionArithmeticOverflow;

	err = mips_cpu_set_register(state, destReg, aluOP[0]);

	if(err == mips_Success)
	{
		//increment PC
		state->pc = state->npc;
		state->npc += 4;		
	}

	return err;
}

mips_error AND(mips_cpu_h state, uint32_t instruction)
{
	unsigned registerA = (instruction << 6) >> 27;
	unsigned registerB = (instruction << 11) >> 27;
	unsigned destReg = (instruction << 16) >> 27;
	
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];

	mips_error err = mips_cpu_get_register(state, registerA, &valA);		
	if(err != mips_Success)
		return err;
	
	err = mips_cpu_get_register(state, registerB, &valB);	
	if(err != mips_Success)
		return err;

	alu_select type = bwand;
	alu_module(type, valA, valB, aluOP);

	err = mips_cpu_set_register(state, destReg, aluOP[0]);

	if(err == mips_Success)
	{
		//increment PC
		state->pc = state->npc;
		state->npc += 4;		
	}

	return err;
}

mips_error OR(mips_cpu_h state, uint32_t instruction)
{
	unsigned registerA = (instruction << 6) >> 27;
	unsigned registerB = (instruction << 11) >> 27;
	unsigned destReg = (instruction << 16) >> 27;
	
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];

	mips_error err = mips_cpu_get_register(state, registerA, &valA);		
	if(err != mips_Success)
		return err;
	
	err = mips_cpu_get_register(state, registerB, &valB);	
	if(err != mips_Success)
		return err;

	alu_select type = bwor;
	alu_module(type, valA, valB, aluOP);

	err = mips_cpu_set_register(state, destReg, aluOP[0]);

	if(err == mips_Success)
	{
		//increment PC
		state->pc = state->npc;
		state->npc += 4;		
	}

	return err;
}

mips_error XOR(mips_cpu_h state, uint32_t instruction)
{
	unsigned registerA = (instruction << 6) >> 27;
	unsigned registerB = (instruction << 11) >> 27;
	unsigned destReg = (instruction << 16) >> 27;
	
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];

	mips_error err = mips_cpu_get_register(state, registerA, &valA);		
	if(err != mips_Success)
		return err;
	
	err = mips_cpu_get_register(state, registerB, &valB);	
	if(err != mips_Success)
		return err;

	alu_select type = bwxor;
	alu_module(type, valA, valB, aluOP);

	err = mips_cpu_set_register(state, destReg, aluOP[0]);

	if(err == mips_Success)
	{
		//increment PC
		state->pc = state->npc;
		state->npc += 4;		
	}

	return err;
}

mips_error i_type(mips_cpu_h state, uint32_t instruction)
{}

mips_error j_type(mips_cpu_h state, uint32_t instruction)
{}

