#include "mips.h"
#include "mips_cpu_decoder.h"
#include "mips_cpu_modules.h"
#include <iostream>
using namespace std;

mips_error r_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
)
{
	instr command;
	unsigned machineWord[5];
	
	split_rtype_word(instruction, machineWord);

	command = (instr) machineWord[4];

	mips_error err = check_rword_validity(command, machineWord);
	if(err != mips_Success)
		return err;

	if((command == jalr) || (command == jr))
	{
		err = execute_r_branch(command, state, machineWord, offset);
		return err;		
	}
	else
	{
		err = execute_r(command, state, machineWord);
		return err;
	}
}

void split_rtype_word(
	uint32_t instruction, 
	unsigned *machineWord
)
{
	machineWord[0] = (instruction << 6) >> 27; //src1
	machineWord[1] = (instruction << 11) >> 27; //src2
	machineWord[2] = (instruction << 16) >> 27; //dest
	machineWord[3] = (instruction << 21) >> 27; //shiftAmt
	machineWord[4] = (instruction << 26) >> 26; //fnCode
}

mips_error check_rword_validity(
	instr command, 
	unsigned* machineWord
)
{
	if((command == addu) || (command == sub) || (command == subu) || (command == bitwiseand) || (command == bitwiseor) || (command == bitwisexor) ||
		(command == sllv) || (command == srav) || (command == srlv) || (command == slt) || (command == sltu))
		command = add;

	else if((command == mthi) || (command == mtlo))
		command = jr;

	else if((command == sra) || (command == srl))
		command = sll;

	else if(command == mflo)
		command = mfhi;

	else if((command == divu) || (command == mult) || (command == multu))
		command = divide;

	switch(command)
	{
		case add:
		{
			//shift 0
			if(machineWord[3] == 0)
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		case sll:
		{
			//src1 0 
			if(machineWord[0] == 0)
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		case jalr:
		{
			//shift 0, src2 0
			if((machineWord[3] == 0) && (machineWord[1] == 0))
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		case jr:
		{
			//shift 0, src2 0, dest 0
			if((machineWord[3] == 0) && (machineWord[1] == 0) && (machineWord[2] == 0))
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		case mfhi:
		{
			//shift 0, src1 0, src2 0
			if((machineWord[3] == 0) && (machineWord[0] == 0) && (machineWord[1] == 0))
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		case divide:
		{
			//shift 0, dest 0
			if((machineWord[3] == 0) && (machineWord[2] == 0))
				return mips_Success;
			else 
				return mips_ExceptionInvalidInstruction;
		}

		default:
		{
			return mips_ExceptionInvalidInstruction;
		}

	}
}

mips_error execute_r(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
)
{
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP[2];
	uint32_t toWrite;

	mips_error err = get_source_registers(state, &valA, machineWord, &valB);
	if(err != mips_Success)
		return err;

	if(type == sll)
	{
		uint32_t shiftAmount = (uint32_t) machineWord[3];
		toWrite = valB << shiftAmount;
	}
	else if(type == sllv)
	{
		uint32_t shiftAmount = valA & 0x1f;
		toWrite = valB << shiftAmount;
	}
	else if(type == sra)
	{
		uint32_t shiftAmount = (uint32_t) machineWord[3];
		uint32_t mask = 0;

		if(valB & 0x80000000)
			mask = 0x80000000;
		
		for(uint32_t i = 1; i<=shiftAmount; i++)
			valB = (valB >> 1) | mask;
		
		toWrite = valB;
	}
	else if(type == srav)
	{
		uint32_t shiftAmount = valA & 0x1f;
		uint32_t mask = 0;

		if(valB & 0x80000000)
			mask = 0x80000000;
		
		for(uint32_t i = 1; i<=shiftAmount; i++)
			valB = (valB >> 1) | mask;
		
		toWrite = valB;			
	}
	else if(type == srl)
	{
		uint32_t shiftAmount = (uint32_t) machineWord[3];
		toWrite = valB >> shiftAmount;
	}
	else if(type == srlv)
	{
		uint32_t shiftAmount = valA & 0x1f;
		toWrite = valB >> shiftAmount;
	}
	else if(type == mfhi)
	{
		err = mips_cpu_get_HILO(state, true, &toWrite);
		if(err != mips_Success)
			return err;
	}
	else if(type == mflo)
	{
		err = mips_cpu_get_HILO(state, false, &toWrite);
		if(err != mips_Success)
			return err;
	}
	else if(type == mthi)
	{
		err = mips_cpu_set_HILO(state, true, valA);
		return err;
	}
	else if(type == mtlo)
	{
		err = mips_cpu_set_HILO(state, false, valA);
		return err;
	}
	else if((type == divide) || (type == divu))
	{
		err = alu_module(type, valA, valB, aluOP);
		if(err != mips_Success)
			return err;

		err = mips_cpu_set_HILO(state, false, aluOP[0]);
		if(err != mips_Success)
			return err;

		err = mips_cpu_set_HILO(state, true, aluOP[1]);
		return err;
	}
	else if((type == mult) || (type == multu))
	{
		err = alu_module(type, valA, valB, aluOP);
		if(err != mips_Success)
			return err;

		err = mips_cpu_set_HILO(state, false, aluOP[0]);
		if(err != mips_Success)
			return err;

		err = mips_cpu_set_HILO(state, true, aluOP[1]);
		return err;
	}
	else
	{
		err = alu_module(type, valA, valB, aluOP);
		if(err != mips_Success)
			return err;

		if(aluOP[1])
			return mips_ExceptionArithmeticOverflow;

		toWrite = aluOP[0];
	}

	err = mips_cpu_set_register(state, machineWord[2], toWrite);

	return err;
}

mips_error execute_r_branch(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset
)
{
	uint32_t valA; 

	mips_error err = get_source_registers(state, &valA, machineWord);
	if(err != mips_Success)
		return err;

	if(type == jalr)
	{
		err = mips_cpu_set_register(state, machineWord[2], state->npc + 4);
		if(err != mips_Success)
			return err;
	}

	*offset = valA - state->npc - 4;

	return err;
}

mips_error i_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
)
{
	instr command; 
	unsigned machineWord[4];

	split_itype_word(instruction, machineWord);
	command = (instr) (machineWord[2] | 0x100);

	if((command == addi) || (command == addiu) || (command == andi) || (command == ori) || (command == xori) || (command == lui) || (command == slti) || (command == sltiu))
	{
		mips_error err = execute_i(command, state, machineWord);
		return err;
	}

	else if ((command == beq) || (command == bne) || (command == bgtz) || (command == blez))
	{
		if((command == bgtz) || (command == blez))
			if(machineWord[1] != 0)
				return mips_ExceptionInvalidInstruction;

		mips_error err = execute_i_branch(command, state, machineWord, offset);
		return err;
	}

	else if(machineWord[2] == 0x1)
	{
		command = (instr) (machineWord[1] | 0x100);
		if((command == bgez) || (command == bltz))
		{
			mips_error err = execute_i_branch(command, state, machineWord, offset);
			return err;
		}
		else
		{
			mips_error err = execute_i_branch(command, state, machineWord, offset, true);
			return err;
		}
	}

	else if((command == lw) || (command == lb) || (command == lbu) || (command == lh) || (command == lhu))
	{
		mips_error err = execute_i_load(command, state, machineWord);
		return err;
	}

	else if((command == sw) || (command == sb) || (command == sh))
	{
		mips_error err = execute_i_store(command, state, machineWord);
		return err;
	}

	else if((command == lwl) || (command == lwr))
	{
		mips_error err = execute_i_loadword_lf(command, state, machineWord);
		return err;
	}

	else 
		return mips_ExceptionInvalidInstruction;
}

void split_itype_word(
	uint32_t instruction, 
	unsigned *machineWord
)
{
	machineWord[2] = (instruction >> 26); //opcode
	machineWord[0] = (instruction << 6) >> 27; //src1
	machineWord[1] = (instruction << 11) >> 27; //src2 or dest
	machineWord[3] = (instruction << 16) >> 16; //address or immediate
}

mips_error execute_i(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
)
{
	uint32_t valA;
	uint32_t valB = (uint32_t) machineWord[3];
	uint32_t aluOP[2];

	if((type != andi) && (type != ori) && (type != xori))
		sign_extend_16_32(&valB);

	if(type == lui)
	{
		if(machineWord[0] != 0)
			return mips_ExceptionInvalidInstruction;
		uint32_t dest = machineWord[1];
		uint32_t input = (machineWord[3] << 16) & 0xffff0000;
		mips_error err = mips_cpu_set_register(state, dest, input);
		return err;	
	}
	else
	{
		mips_error err = get_source_registers(state, &valA, machineWord);
		if(err != mips_Success)
			return err;

		err = alu_module(type, valA, valB, aluOP);
		if(err != mips_Success)
			return err;

		if(aluOP[1])
			return mips_ExceptionArithmeticOverflow;

		err = mips_cpu_set_register(state, machineWord[1], aluOP[0]);

		return err;
	}

	// if(err == mips_Success)
	// {
	// 	//increment PC
	// 	state->pc = state->npc;
	// 	state->npc += 4;		
	// }
}

mips_error execute_i_branch(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset, 
	bool link
)
{
	uint32_t valA;
	uint32_t valB;
	uint32_t aluOP;
	uint32_t temp = (uint32_t) machineWord[3];

	//calculate value to which to branch
	sign_extend_16_32(&temp);

	mips_error err = get_source_registers(state, &valA, machineWord, &valB);
	if(err != mips_Success)
		return err;

	err = alu_module(type, valA, valB, &aluOP);
	if(err != mips_Success)
		return err;

	if(link)
	{
		err = mips_cpu_set_register(state, 31, state->pc + 8);
		if(err != mips_Success)
			return err;
	}

	//check if branch is to be taken
	if(aluOP)
	{
		//4 is subtracted from the offset as on return, offset is added to npc which has already been incremented by 4
		*offset = (temp << 2) - 4;
		return err;
	}

	return err;
}

mips_error execute_i_load(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
)
{
	uint32_t valA;
	uint32_t valB = (uint32_t) machineWord[3];
	uint32_t aluOP[2];
	uint32_t addressToRead, byteToWrite, data;

	mips_error err = get_source_registers(state, &valA, machineWord);
	if(err != mips_Success)
		return err;

	sign_extend_16_32(&valB);

	err = alu_module(type, valA, valB, aluOP);
	if(err != mips_Success)
		return err;

	if((type == lb) || (type == lbu) || (type == lh) || (type == lhu))
	{
		addressToRead = (aluOP[0]/4) * 4;
		byteToWrite = aluOP[0]%4;

		if((type == lh) || (type == lhu))
		{
			if((byteToWrite == 1) || (byteToWrite == 3))
				return mips_ExceptionInvalidAddress;
		}
	}
	else
		addressToRead = aluOP[0];

	uint8_t dataInBuffer[4];
	err = mips_mem_read(state->ram, addressToRead, 4, dataInBuffer);
	if(err != mips_Success)
		return err;

	if(type == lb)
	{
		data = (uint32_t) dataInBuffer[byteToWrite];
		sign_extend_8_32(&data);
	}
	else if(type == lbu)
	{
		data = (uint32_t) dataInBuffer[byteToWrite];
		data = 0x000000ff & data;
	}
	else if(type == lh)
	{
		data = ((uint32_t(dataInBuffer[byteToWrite]) << 8) | uint32_t(dataInBuffer[byteToWrite+1]));
		sign_extend_16_32(&data);
	}
	else if(type == lhu)
	{
		data = ((uint32_t(dataInBuffer[byteToWrite]) << 8) | uint32_t(dataInBuffer[byteToWrite+1]));
		data = 0x0000ffff & data;
	}
	else
		data = to_little_endian(dataInBuffer);

	err = mips_cpu_set_register(state, machineWord[1], data);
	return err;
}

mips_error execute_i_loadword_lf(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
)
{
	uint32_t valA;
	uint32_t valB = (uint32_t) machineWord[3];
	uint32_t aluOP[2];
	uint32_t addressToRead, byteToWrite, data, destReg;

	mips_error err = get_source_registers(state, &valA, machineWord);
	if(err != mips_Success)
		return err;

	sign_extend_16_32(&valB);

	err = alu_module(type, valA, valB, aluOP);
	if(err != mips_Success)
		return err;
	
	addressToRead = (aluOP[0]/4) * 4;
	byteToWrite = aluOP[0]%4;

	uint8_t dataInBuffer[4], writeRegs[4];
	err = mips_mem_read(state->ram, addressToRead, 4, dataInBuffer);
	if(err != mips_Success)
		return err;

	err = mips_cpu_get_register(state, machineWord[1], &destReg);
	if(err != mips_Success)
		return err;

	to_big_endian(destReg, writeRegs);

	if(type == lwl)
	{
		int j = 0;
		for(int i = byteToWrite; i<=3; i++)
		{
			writeRegs[j] = dataInBuffer[i];
			j++;
		}

		data = to_little_endian(writeRegs);
	}

	else
	{
		int j = 3;
		for (int i = byteToWrite; i>=0; i--)
		{
			writeRegs[j] = dataInBuffer[i];
			j--;
		}

		data = to_little_endian(writeRegs);
	}

	err = mips_cpu_set_register(state, machineWord[1], data);
	return err;
}

mips_error execute_i_store(
	instr type, 
	mips_cpu_h state, 
	unsigned *machineWord
)
{
	uint32_t valA;
	uint32_t valB;
	uint32_t imm = (uint32_t) machineWord[3];
	uint32_t aluOP[2];
	uint8_t dataOutBuffer[4], addressToRead, byteToWrite;

	mips_error err = get_source_registers(state, &valA, machineWord, &valB);
	if(err != mips_Success)
		return err;

	sign_extend_16_32(&imm);

	err = alu_module(type, valA, imm, aluOP);
	if(err != mips_Success)
		return err;

	if((type == sb) || (type == sh))
	{
		addressToRead = (aluOP[0]/4) * 4;
		byteToWrite = aluOP[0]%4;


		if(type == sh)
			if((byteToWrite == 1) || (byteToWrite == 3))
				return mips_ExceptionInvalidAddress;

		uint8_t dataInBuffer[4];
		err = mips_mem_read(state->ram, addressToRead, 4, dataInBuffer);
		if(err != mips_Success)
			return err;

		if(type == sh)
		{
			dataInBuffer[byteToWrite] = (valB & 0xff00) >> 8;
			dataInBuffer[byteToWrite+1] = (valB & 0xff);
		}
		else 
			dataInBuffer[byteToWrite] = valB & 0xff;

		for(int i = 0; i<4; i++)
		{
			dataOutBuffer[i] = dataInBuffer[i];
		}
	}

	else
	{
		addressToRead = aluOP[0];
		to_big_endian(valB, dataOutBuffer);
	}

	err = mips_mem_write(state->ram, addressToRead, 4, dataOutBuffer);

	return err;
}

mips_error j_type(
	mips_cpu_h state, 
	uint32_t instruction, 
	uint32_t *offset
)
{
	unsigned machineWord[2];

	split_jtype_word(instruction, machineWord);

	if(machineWord[0] == 0x2)
	{
		mips_error err = execute_j(state, machineWord, offset);
		return err;
	}

	else if(machineWord[0] == 0x3)
	{
		mips_error err = execute_j(state, machineWord, offset, true);
		return err;
	}
	else 
		return mips_ExceptionInvalidInstruction;
	
}
void split_jtype_word(
	uint32_t instruction, 
	unsigned *machineWord
)
{
	machineWord[0] = instruction >> 26;
	machineWord[1] = (instruction << 6) >> 6;
}

mips_error execute_j(
	mips_cpu_h state, 
	unsigned *machineWord, 
	uint32_t *offset, 
	bool link
)
{
	//target address bottom 28 bits : bottom 26 bits of instruction word shifted left by 2
	//target address to 4 bits : top 4 bits of address in npc
	uint32_t targetAddress = (((uint32_t) machineWord[1]) << 2) | (((state->npc) >> 28) << 28);

	//link if JAL
	if(link)
	{
		mips_error err = mips_cpu_set_register(state, 31, state->pc + 8);
		if(err != mips_Success)
			return err;
	} 

	*offset = targetAddress - state->npc - 4;

	return mips_Success;
}

mips_error get_source_registers(
	mips_cpu_h state, 
	uint32_t *valA, 
	unsigned *machineWord, 
	uint32_t *valB
)
{
	mips_error err = mips_cpu_get_register(state, machineWord[0], valA);		
	if(err != mips_Success)
		return err;
	
	if(valB != NULL)
	{
		err = mips_cpu_get_register(state, machineWord[1], valB);	
		if(err != mips_Success)
			return err;
	}

	return mips_Success;
}


