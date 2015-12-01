#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "mips.h"

using namespace std;

void log_error(ofstream &logfile, string testName, string message, mips_error err);

int main()
{
	uint8_t data[4];
	uint32_t instruction;
	uint32_t buffer;
	uint32_t expected;
	int testID;
	bool outcome; 
	mips_error err;
	ofstream logger;
	string testName;

	logger.open("test.log");

	mips_mem_h ram = mips_mem_create_ram(4096, 4);
	mips_cpu_h cpu1 = mips_cpu_create(ram);

	mips_test_begin_suite();
	
	//********************************* ADD *********************************  
	testName = "add basic - 5+10";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//add $5, $3, $4
	instruction = (0ul << 26) | (3ul << 21) | (4ul << 16) | (5ul << 11) | (0ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers to test
	err = mips_cpu_set_register(cpu1, 3, 5);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 4, 10);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep returned exception: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 15;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADD *********************************  
	testName = "add positive with negative 5 + 0xffffff6a";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	//set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup memory with instruction
	//add $25, $23, $24
	instruction = (0ul << 26) | (23ul << 21) | (24ul << 16) | (25ul << 11) | (0ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 5);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 24, 0xffffff6a);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep returned exception: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 25, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xffffff6f;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADD *********************************  
	testName = "add negatives 0xffff3452 + 0xfffe2345";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	//set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//add $13, $14, $15
	instruction = (0ul << 26) | (14ul << 21) | (15ul << 16) | (13ul << 11) | (0ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 14, 0xffff3452);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 15, 0xfffe2345);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep returned exception: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xfffd5797;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADD *********************************  
	testName = "add large positives - 0x7fffffff + 0x7eeefeee";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	//set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory 
	//add $12, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (12ul << 11) | (0ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x7eeefeee);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 12, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionArithmeticOverflow)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep returned exception: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 12, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADD *********************************  
	testName = "add - bad instruction - shift non 0";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	//set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory  
	//add $12, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (12ul << 11) | (5ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 10, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x7eeefeee);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 12, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep returned exception: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 12, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADD *********************************  
	testName = "add instruction - write to $0";
	testID = mips_test_begin_test("ADD");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//add $0, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (0ul << 11) | (0ul << 6) | 0x20;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0xfeeefeee);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 0, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x0;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDU *********************************  
	testName = "addu large positives 0x7fffffff + 0x7eeefeee";
	testID = mips_test_begin_test("ADDU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addu $14, $8, $9
	instruction = (0ul << 26) | (8ul << 21) | (9ul << 16) | (14ul << 11) | (0ul << 6) | 0x21;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 9, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x7eeefeee);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xfeeefeed;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDU *********************************  
	testName = "addu large negatives 0x80000008 + 0x80000246";
	testID = mips_test_begin_test("ADDU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addu $14, $8, $9
	instruction = (0ul << 26) | (8ul << 21) | (9ul << 16) | (14ul << 11) | (0ul << 6) | 0x21;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 9, 0x80000008);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x80000246);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x24e;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDI *********************************  
	testName = "addi large positives - 0x7fff342d + 0x7fff";
	testID = mips_test_begin_test("ADDI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addi $14, $8, 0x7fff
	instruction = (0x8ul << 26) | (8ul << 21) | (14ul << 16) | 0x7fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff342d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7fffb42c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDI *********************************  
	testName = "addi - check if immediate is sign extended - 0x7fff342d + 0xffff8fff";
	testID = mips_test_begin_test("ADDI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addi $14, $8, 0x8fff
	instruction = (0x8ul << 26) | (8ul << 21) | (14ul << 16) | 0x8fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff342d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7ffec42c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDI *********************************  
	testName = "addi - overflow - 0x7fff942d + 0x7fff";
	testID = mips_test_begin_test("ADDI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addi $14, $8, 0x7fff
	instruction = (0x8ul << 26) | (8ul << 21) | (14ul << 16) | 0x7fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff942d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionArithmeticOverflow)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDIU *********************************  
	testName = "addiu - positives 0x7fff342d + 0x7fff";
	testID = mips_test_begin_test("ADDIU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addiu $14, $8, 0x7fff
	instruction = (0x9ul << 26) | (8ul << 21) | (14ul << 16) | 0x7fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff342d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7fffb42c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDIU *********************************  
	testName = "addi - check if immediate is sign extended";
	testID = mips_test_begin_test("ADDIU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addiu $14, $8, 0x8fff
	instruction = (0x9ul << 26) | (8ul << 21) | (14ul << 16) | 0x8fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff342d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7ffec42c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ADDIU *********************************  
	testName = "addiu - no overflow test - 0x7fff942d + 0x7fff";
	testID = mips_test_begin_test("ADDIU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//addiu $14, $8, 0x7fff
	instruction = (0x9ul << 26) | (8ul << 21) | (14ul << 16) | 0x7fff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 8, 0x7fff942d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 14, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x8000142c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}


	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUB *********************************  
	testName = "sub basic - 12 - 5";
	testID = mips_test_begin_test("SUB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sub $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (0ul << 6) | 0x22;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0xa);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x5);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x5;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUB *********************************  
	testName = "subtract small positive and larger positive - (5 - 0x96)";
	testID = mips_test_begin_test("SUB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sub $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (0ul << 6) | 0x22;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0x5);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x96);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xffffff6f;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUB *********************************  
	testName = "sub large negative with positive - (0xf0000008 - 0x7ffffe8c)";
	testID = mips_test_begin_test("SUB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sub $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (0ul << 6) | 0x22;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0xf0000008);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x7ffffe8c);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 22, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionArithmeticOverflow)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUB *********************************  
	testName = "subtract large negatives - (0x80000008 - 0x80000342)";
	testID = mips_test_begin_test("SUB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sub $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (0ul << 6) | 0x22;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0x80000008);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x80000342);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xfffffcc6;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUB *********************************  
	testName = "subtract - bad instruction";
	testID = mips_test_begin_test("SUB");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sub $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (8ul << 6) | 0x22;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0x80000008);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x80000342);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 22, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SUBU *********************************  
	testName = "subtract unsigned - bad instruction";
	testID = mips_test_begin_test("SUBU");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//subu $22, $10, $11
	instruction = (0ul << 26) | (10ul << 21) | (11ul << 16) | (22ul << 11) | (0ul << 6) | 0x23;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 10, 0xf0000008);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 11, 0x7ffffe8c);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 22, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7000017c;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* AND *********************************  
	testName = "bitwise and";
	testID = mips_test_begin_test("AND");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//and $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (0ul << 6) | 0x24;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x4f42483;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* AND *********************************  
	testName = "bitwise and - bad instruction";
	testID = mips_test_begin_test("AND");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//and $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (5ul << 6) | 0x24;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* OR *********************************  
	testName = "bitwise or";
	testID = mips_test_begin_test("OR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//or $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (0ul << 6) | 0x25;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x7dffbed3;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* OR *********************************  
	testName = "bitwise or - bad instruction";
	testID = mips_test_begin_test("OR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//or $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (5ul << 6) | 0x25;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* XOR *********************************  
	testName = "bitwise xor";
	testID = mips_test_begin_test("XOR");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//xor $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x790b9a50;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* XOR *********************************  
	testName = "bitwise xor bad instruction";
	testID = mips_test_begin_test("XOR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//xor $30, $4, $13
	instruction = (0ul << 26) | (4ul << 21) | (13ul << 16) | (30ul << 11) | (5ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x34f53cd3);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 13, 0x4dfea683);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 30, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* XORI *********************************  
	testName = "bitwise xor immediate";
	testID = mips_test_begin_test("XORI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//xori $26, $7, 0xf034
	instruction = (0xeul << 26) | (7ul << 21) | (26ul << 16) | 0xf034;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 26, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xe123bc69;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ORI *********************************  
	testName = "bitwise or immediate";
	testID = mips_test_begin_test("ORI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//ori $26, $7, 0xf034
	instruction = (0xdul << 26) | (7ul << 21) | (26ul << 16) | 0xf034;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 26, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xe123fc7d;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* ANDI *********************************  
	testName = "bitwise and immediate";
	testID = mips_test_begin_test("ANDI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//andi $26, $7, 0xf034
	instruction = (0xcul << 26) | (7ul << 21) | (26ul << 16) | 0xf034;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 26, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x4014;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLLV *********************************  
	testName = "shift left logical variable";
	testID = mips_test_begin_test("SLLV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sllv $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (0ul << 6) | 0x4;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = ((0xe1234c5d) << 19);
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLLV *********************************  
	testName = "shift left logical variable - bad instruction";
	testID = mips_test_begin_test("SLLV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sllv $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (1ul << 6) | 0x4;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRLV *********************************  
	testName = "shift right logical variable";
	testID = mips_test_begin_test("SRLV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srlv $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (0ul << 6) | 0x6;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333914);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = ((0xe1234c5d) >> 20);
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRLV *********************************  
	testName = "shift right logical variable - bad instruction";
	testID = mips_test_begin_test("SRLV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srlv $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (1ul << 6) | 0x4;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu`
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRAV *********************************  
	testName = "shift right arithmetic variable - with sign extend";
	testID = mips_test_begin_test("SRAV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srav $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (0ul << 6) | 0x7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xfffffc24;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRAV *********************************  
	testName = "shift right arithmetic variable - without sign extend";
	testID = mips_test_begin_test("SRAV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srav $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (0ul << 6) | 0x7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x71234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0xe24;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRAV *********************************  
	testName = "shift right arithmetic variable - bad instruction";
	testID = mips_test_begin_test("SRAV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srav $23, $7, $8
	instruction = (0ul << 26) | (8ul << 21) | (7ul << 16) | (23ul << 11) | (5ul << 6) | 0x7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x42333913);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* SLT *********************************  
	testName = "set less than - large negative with large positive";
	testID = mips_test_begin_test("SLT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slt $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (0ul << 6) | 0x2a;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x8000fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x7fffde24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 1ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLT *********************************  
	testName = "set less than - two negative numbers";
	testID = mips_test_begin_test("SLT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slt $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (0ul << 6) | 0x2a;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xfff0fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0xf000de24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLT *********************************  
	testName = "set less than - bad instruction";
	testID = mips_test_begin_test("SLT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slt $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (6ul << 6) | 0x2a;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xfff0fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0xf000de24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTU *********************************  
	testName = "set less than unsigned - large negative with large positive";
	testID = mips_test_begin_test("SLTU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sltu $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (0ul << 6) | 0x2b;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x8000fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0x7fffde24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTU *********************************  
	testName = "set less than unsigned - two negative numbers";
	testID = mips_test_begin_test("SLTU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sltu $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (0ul << 6) | 0x2b;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xf000de24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0xfff0fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 1ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTU *********************************  
	testName = "set less than - bad instruction";
	testID = mips_test_begin_test("SLTU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sltu $23, $7, $8
	instruction = (0ul << 26) | (7ul << 21) | (8ul << 16) | (23ul << 11) | (6ul << 6) | 0x2b;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xfff0fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 8, 0xf000de24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTI *********************************  
	testName = "set less than immediate - two negatives";
	testID = mips_test_begin_test("SLTI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slti $23, $7, 0x80f3
	instruction = (0xaul << 26) | (7ul << 21) | (23ul << 16) | 0x80f3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xffff80da);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 1ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTI *********************************  
	testName = "set less than immediate - positive immediate";
	testID = mips_test_begin_test("SLTI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slti $23, $7, 0x7fed
	instruction = (0xaul << 26) | (7ul << 21) | (23ul << 16) | 0x7fed;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xf000de24);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 1ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTI *********************************  
	testName = "set less than immediate - write to $0";
	testID = mips_test_begin_test("SLTI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//slti $0, $7, 0xffe3
	instruction = (0xaul << 26) | (7ul << 21) | (0ul << 16) | 0xffe3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xfff0fe23);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 0, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTIU *********************************  
	testName = "set less than immediate unsigned - check if immediate is sign extended";
	testID = mips_test_begin_test("SLTIU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sltiu $23, $7, 0x80f3
	instruction = (0xbul << 26) | (7ul << 21) | (23ul << 16) | 0x80f3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xffff80da);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 1ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLTIU *********************************  
	testName = "set less than immediate unsigned - basic";
	testID = mips_test_begin_test("SLTIU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sltiu $23, $7, 0x70f3
	instruction = (0xbul << 26) | (7ul << 21) | (23ul << 16) | 0x70f3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xffff80da);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LUI *********************************  
	testName = "load upper immediate";
	testID = mips_test_begin_test("LUI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lui $21, 0x8df3
	instruction = (0xful << 26) | (0ul << 21) | (21ul << 16) | 0x8df3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 21, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x8df30000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LUI - bad instruction *********************************  
	testName = "load upper immediate - bad instruction";
	testID = mips_test_begin_test("LUI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lui $21, 0x8df3
	instruction = (0xful << 26) | (3ul << 21) | (21ul << 16) | 0x8df3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 21, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);
	
	expected = 0x8df30000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLL *********************************  
	testName = "shift left logical";
	testID = mips_test_begin_test("SLL");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sll $23, $7, 0xf
	instruction = (0ul << 26) | (0ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x0;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xa62e8000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SLL - bad instruction  *********************************  
	testName = "shift left logical - bad instruction";
	testID = mips_test_begin_test("SLL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sll $23, $7, 0xf
	instruction = (0ul << 26) | (5ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x0;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRA *********************************  
	testName = "shift right arithmetic - MSB 1";
	testID = mips_test_begin_test("SRA");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sra $23, $7, 0xf
	instruction = (0ul << 26) | (0ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0xe1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffc246;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRA *********************************  
	testName = "shift right arithmetic - MSB 0";
	testID = mips_test_begin_test("SRA");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sra $23, $7, 0xf
	instruction = (0ul << 26) | (0ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x3;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x71234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe246;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRL *********************************  
	testName = "shift right logical";
	testID = mips_test_begin_test("SRL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srl $23, $7, 0xf
	instruction = (0ul << 26) | (0ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x2;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x71234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe246;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SRL *********************************  
	testName = "shift right logical - MSB is 1";
	testID = mips_test_begin_test("SRL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//srl $23, $7, 0xf
	instruction = (0ul << 26) | (0ul << 21) | (7ul << 16) | (23ul << 11) | (0xf << 6) | 0x2;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 7, 0x81234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 23, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x10246;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BEQ *********************************
	testName = "branch on equal - branch case";
	testID = mips_test_begin_test("BEQ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//beq $21, $23, -9
	instruction = (4ul << 26) | (21ul << 21) | (23ul << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 21, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{	
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x20;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BNE *********************************
	testName = "branch not equal - branch case";
	testID = mips_test_begin_test("BNE");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bne $21, $23, -9
	instruction = (5ul << 26) | (21ul << 21) | (23ul << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//nop 
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (0ul << 11) | (0ul << 6) | 0;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 21, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0x4432);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{	
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x20;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BNE *********************************
	testName = "branch not equal - no branch case";
	testID = mips_test_begin_test("BNE");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bne $21, $23, -9
	instruction = (5ul << 26) | (21ul << 21) | (23ul << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//nop 
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (0ul << 11) | (0ul << 6) | 0;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 21, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 23, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{	
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BLEZ *********************************
	testName = "branch on less than equal to zero - branch case";
	testID = mips_test_begin_test("BLEZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//blez $21, 9
	instruction = (6ul << 26) | (21ul << 21) | (0ul << 16) | 0x9;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 21, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x68;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BLEZ - bad instruction *********************************
	testName = "branch on less than equal to zero - bad instruction";
	testID = mips_test_begin_test("BLEZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//blez $21, 9
	instruction = (6ul << 26) | (21ul << 21) | (5ul << 16) | 0x9;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 21, 0xf1234c5d);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x40;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BLTZ *********************************
	testName = "branch on less than zero - branch case";
	testID = mips_test_begin_test("BLTZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bltz $21, 9
	instruction = (1ul << 26) | (21ul << 21) | (0ul << 16) | 0x9;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	
	err = mips_cpu_set_register(cpu1, 21, 0x0);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* BGTZ *********************************
	testName = "branch on greater than zero - no branch case ";
	testID = mips_test_begin_test("BGTZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bgtz $21, 9
	instruction = (7ul << 26) | (21ul << 21) | (0ul << 16) | 0x9;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	
	err = mips_cpu_set_register(cpu1, 21, 0x0);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BGEZ *********************************
	testName = "branch on greater than or equal to zero - branch case";
	testID = mips_test_begin_test("BGEZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bgez $0, -9
	instruction = (1ul << 26) | (0ul << 21) | (0x1 << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x20;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BGEZ *********************************
	testName = "branch on greater than or equal to zero - no branch case";
	testID = mips_test_begin_test("BGEZ");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bgez $3, -9
	instruction = (1ul << 26) | (3ul << 21) | (0x1 << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	
	err = mips_cpu_set_register(cpu1, 3, 0x80003400);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* BGEZAL *********************************
	testName = "branch on greater than zero and link - no branch case";
	testID = mips_test_begin_test("BGEZAL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bgezal $21, -9
	instruction = (1ul << 26) | (21ul << 21) | (0x11 << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	
	err = mips_cpu_set_register(cpu1, 21, 0xffffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 31, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "return address: " << hex << buffer << endl;
		toPrint << "expected returned address: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BLTZAL *********************************
	testName = "branch on less than zero and link - branch case";
	testID = mips_test_begin_test("BLTZAL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bltzal $21, -9
	instruction = (1ul << 26) | (21ul << 21) | (0x10 << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	
	err = mips_cpu_set_register(cpu1, 21, 0xffffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 31, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "return address: " << hex << buffer << endl;
		toPrint << "expected returned address: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x20;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* BLTZAL *********************************
	testName = "branch on less than zero and link - no branch case";
	testID = mips_test_begin_test("BLTZAL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//bltzal $0, -9
	instruction = (1ul << 26) | (0ul << 21) | (0x10 << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 31, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "return address: " << hex << buffer << endl;
		toPrint << "expected returned address: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LB *********************************
	testName = "load byte - check if byte is sign extended";
	testID = mips_test_begin_test("LB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lb $15, 5($3 = 25)
	instruction = (32ul << 26) | (3ul << 21) | (15ul << 16) | 5ul;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0xff;
	data[3] = 0x52;
	err = mips_mem_write(ram, 0x1c, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 15, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffffff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LB *********************************
	testName = "load byte - check if offset is signed";
	testID = mips_test_begin_test("LB");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lb $15, -1($3 = 31)
	instruction = (32ul << 26) | (3ul << 21) | (15ul << 16) | 0xffff;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x1c, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 31ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 15, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x52;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LBU *********************************
	testName = "load byte unsigned - check if no sign extension";
	testID = mips_test_begin_test("LBU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lb $15, -2($3 = 31)
	instruction = (0x24ul << 26) | (3ul << 21) | (15ul << 16) | 0xfffe;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x1c, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 31ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 15, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe3;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LBU *********************************
	testName = "load byte unsigned";
	testID = mips_test_begin_test("LBU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lb $15, 0($3 = 31)
	instruction = (0x24ul << 26) | (3ul << 21) | (15ul << 16) | 0x0;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x1c, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 31ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 15, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* LH *********************************
	testName = "load halfword - check if halfword is sign extended";
	testID = mips_test_begin_test("LH");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $13, -3($3 = 25)
	instruction = (0x21ul << 26) | (3ul << 21) | (13ul << 16) | 0xfffd;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0xff;
	data[3] = 0x52;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffff52;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);
	
	//********************************* LH *********************************
	testName = "load halfword - incorrect byte referencing";
	testID = mips_test_begin_test("LH");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $13, -4($3 = 25)
	instruction = (0x21ul << 26) | (3ul << 21) | (13ul << 16) | 0xfffc;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 13, 0x12345678);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidAddress)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x12345678;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LHU *********************************
	testName = "load halfword unsigned - make sure halfword doesn't sign extended";
	testID = mips_test_begin_test("LHU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $13, -5($3 = 25)
	instruction = (0x25ul << 26) | (3ul << 21) | (13ul << 16) | 0xfffb;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0xe3;
	data[1] = 0x45;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe345;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LHU *********************************
	testName = "load halfword unsigned - basic";
	testID = mips_test_begin_test("LHU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $13, 7($3 = 15)
	instruction = (0x25ul << 26) | (3ul << 21) | (13ul << 16) | 0x7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0xe3;
	data[1] = 0x45;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 15ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x52ff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LW *********************************
	testName = "load word - basic";
	testID = mips_test_begin_test("LW");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $13, -5($3 = 25)
	instruction = (0x23ul << 26) | (3ul << 21) | (13ul << 16) | 0xfffb;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 13, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x45e352ff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);


	//********************************* LW *********************************
	testName = "load word - load to $0";
	testID = mips_test_begin_test("LW");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lh $0, -5($3 = 25)
	instruction = (0x23ul << 26) | (3ul << 21) | (0ul << 16) | 0xfffb;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 0, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x0;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SB *********************************
	testName = "store byte - basic";
	testID = mips_test_begin_test("SB");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $5, 2($3 = 20)
	instruction = (0x28ul << 26) | (3ul << 21) | (5ul << 16) | 0x2;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 20ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 5, 0x2352de1f);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x45e31fff;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SB *********************************
	testName = "store byte - check if offset is sign extended";
	testID = mips_test_begin_test("SB");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $0, -2($3 = 25)
	instruction = (0x28ul << 26) | (3ul << 21) | (0ul << 16) | 0xfffe;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x45e35200;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SH *********************************
	testName = "store halfword - word start alligned";
	testID = mips_test_begin_test("SH");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $5, -5($3 = 25)
	instruction = (0x29ul << 26) | (3ul << 21) | (5ul << 16) | 0xfffb;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 5, 0xfe3324ab);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x24ab52ff;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SH *********************************
	testName = "store halfword - halfword alligned";
	testID = mips_test_begin_test("SH");
	
	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $5, -3($3 = 25)
	instruction = (0x29ul << 26) | (3ul << 21) | (5ul << 16) | 0xfffd;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 5, 0xfe3324ab);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x45e324ab;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SH *********************************
	testName = "store halfword - bad address";
	testID = mips_test_begin_test("SH");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $5, -2($3 = 25)
	instruction = (0x29ul << 26) | (3ul << 21) | (5ul << 16) | 0xfffe;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x45;
	data[1] = 0xe3;
	data[2] = 0x52;
	data[3] = 0xff;
	err = mips_mem_write(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 5, 0xfe3324ab);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidAddress)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x45e352ff;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SW *********************************
	testName = "store word - read from $0";
	testID = mips_test_begin_test("SW");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $0, 20($0)
	instruction = (0x2bul << 26) | (0ul << 21) | (0ul << 16) | 0x14;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 25ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0x00000000;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* SW *********************************
	testName = "store word - negative offset";
	testID = mips_test_begin_test("SW");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//sb $5, -10($3 = 30)
	instruction = (0x2bul << 26) | (3ul << 21) | (5ul << 16) | 0xfff6;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 3, 30ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 5, 0xf0d30452);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_mem_read(ram, 0x14, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memRead failed with error: ", err);

	expected = 0xf0d30452;
	buffer = ((uint32_t(data[0]) << 24) | (uint32_t(data[1]) << 16) | (uint32_t(data[2]) << 8) | (uint32_t(data[3]) << 0));
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWL ********************************* 
	testName = "load word left - byte 0";
	testID = mips_test_begin_test("LWL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $4, 0x1c($0)
	instruction = (0x22ul << 26) | (0ul << 21) | (4ul << 16) | 0x1c;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x1c, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x2325ffea;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWL ********************************* 
	testName = "load word left - byte 1";
	testID = mips_test_begin_test("LWL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $4, 0x1d($0)
	instruction = (0x22ul << 26) | (0ul << 21) | (4ul << 16) | 0x1d;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x1c, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x25ffea11;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWL ********************************* 
	testName = "load word left - byte 2";
	testID = mips_test_begin_test("LWL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $4, 0x1e($0)
	instruction = (0x22ul << 26) | (0ul << 21) | (4ul << 16) | 0x1e;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x1c, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffeacd11;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWL ********************************* 
	testName = "load word left - byte 3";
	testID = mips_test_begin_test("LWL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $4, 0x1f($0)
	instruction = (0x22ul << 26) | (0ul << 21) | (4ul << 16) | 0x1f;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x1c, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xeaeacd11;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWR ********************************* 
	testName = "load word right - byte 3";
	testID = mips_test_begin_test("LWR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $1, -7($4 = 30)
	instruction = (0x26ul << 26) | (4ul << 21) | (1ul << 16) | 0xfff9;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x14, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 0x1e);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 1, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x2325ffea;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWR ********************************* 
	testName = "load word right - byte 2";
	testID = mips_test_begin_test("LWR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $1, -8($4 = 30)
	instruction = (0x26ul << 26) | (4ul << 21) | (1ul << 16) | 0xfff8;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x14, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 30ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 1, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x452325ff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWR ********************************* 
	testName = "load word right - byte 1";
	testID = mips_test_begin_test("LWR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $1, -9($4 = 30)
	instruction = (0x26ul << 26) | (4ul << 21) | (1ul << 16) | 0xfff7;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x14, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 30ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 1, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x45ea2325;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* LWR ********************************* 
	testName = "load word right - byte 0";
	testID = mips_test_begin_test("LWR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//lwl $1, -10($4 = 30)
	instruction = (0x26ul << 26) | (4ul << 21) | (1ul << 16) | 0xfff6;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	data[0] = 0x23;
	data[1] = 0x25;
	data[2] = 0xff;
	data[3] = 0xea;
	err = mips_mem_write(ram, 0x14, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 30ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	err = mips_cpu_set_register(cpu1, 1, 0x45eacd11);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_register(cpu1, 1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x45eacd23;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JUMP *********************************
	testName = "jump";
	testID = mips_test_begin_test("J");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//j 1000
	instruction = (2ul << 26) | 0x3e8;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{	
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xfa0;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JAL *********************************
	testName = "jump and link";
	testID = mips_test_begin_test("JAL");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jal 1000
	instruction = (3ul << 26) | 0x3e8;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);

	//setup registers
	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 31, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "return address: " << hex << buffer << endl;
		toPrint << "expected returned address: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xfa0;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JALR *********************************
	testName = "jump and link register - non 31 link register";
	testID = mips_test_begin_test("JALR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jalr $7, $4 = 3096
	instruction = (0ul << 26) | (4ul << 21) | (0ul << 16) | (7ul << 11) | (0ul << 6) | 0x9;	
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 3096ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}
	//test link register stored
	err = mips_cpu_get_register(cpu1, 7, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x48;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "return address: " << hex << buffer << endl;
		toPrint << "expected returned address: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 3096ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JALR *********************************
	testName = "jump and link register - bad instruction";
	testID = mips_test_begin_test("JALR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jalr $7, $4 = 3096
	instruction = (0ul << 26) | (4ul << 21) | (5ul << 16) | (7ul << 11) | (0ul << 6) | 0x9;	
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 3096ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);		

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JR *********************************
	testName = "jump register";
	testID = mips_test_begin_test("JR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jr $4 = 3096
	instruction = (0ul << 26) | (4ul << 21) | (0ul << 16) | (0ul << 11) | (0ul << 6) | 0x8;	
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 3096ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 3096ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JR *********************************
	testName = "jump register - exception on unalligned address";
	testID = mips_test_begin_test("JR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jr $4 = 3097
	instruction = (0ul << 26) | (4ul << 21) | (0ul << 16) | (0ul << 11) | (0ul << 6) | 0x8;	
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//xor $5, $1, $2
	instruction = (0ul << 26) | (1ul << 21) | (2ul << 16) | (5ul << 11) | (0ul << 6) | 0x26;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x44, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 3097ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 1, 0xf);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	err = mips_cpu_set_register(cpu1, 2, 0x1);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);	

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep1 failed with error: ", err);
		outcome = false;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x44;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep2 failed with error: ", err);
		outcome = false;
	}
	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidAlignment)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep3 failed with error: ", err);
		outcome = true;
	}

	//test result
	err = mips_cpu_get_pc(cpu1, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 3097ul;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "current pc: " << hex << buffer << endl;
		toPrint << "expected pc: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 5, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xe;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in buffer: " << hex << buffer << endl;
		toPrint << "expected: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* JR *********************************
	testName = "jump register - bad instruction";
	testID = mips_test_begin_test("JR");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0x40);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//jr $4 = 3096
	instruction = (0ul << 26) | (4ul << 21) | (5ul << 16) | (0ul << 11) | (0ul << 6) | 0x8;	
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0x40, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 4, 3096ul);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);		

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* DIV *********************************  
	testName = "division - basic  9 / -2";
	testID = mips_test_begin_test("DIV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//div $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x1a;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0x9);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0xfffffffe);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 1;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xfffffffc;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* DIV *********************************  
	testName = "division - basic - -9 / 2";
	testID = mips_test_begin_test("DIV");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//div $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x1a;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xfffffff7);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x2);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffffff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xfffffffc;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* DIVU *********************************  
	testName = "division unsigned 0xfffffff7 / 2";
	testID = mips_test_begin_test("DIVU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//div $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x1b;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xfffffff7);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x2);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 1;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x7ffffffb;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* DIVU *********************************  
	testName = "division unsigned 0xfffffff7 / 0x7fff0394";
	testID = mips_test_begin_test("DIVU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//div $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x1b;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xfffffff7);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x7fff0394);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x1f8cf;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x2;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MULT *********************************  
	testName = "multiplication - basic - -5 * 12";
	testID = mips_test_begin_test("MULT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mult $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x18;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xfffffffb);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0xc);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffffff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xffffffc4;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MULT *********************************  
	testName = "multiplication large positives - 0x7fffffff * 0x7fffffff";
	testID = mips_test_begin_test("MULT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mult $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x18;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x7fffffff);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x3fffffff;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x00000001;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MULT *********************************  
	testName = "multiplication large negatives - 0xf00000ef * 0xe9df0000";
	testID = mips_test_begin_test("MULT");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mult $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x18;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xf00000ef);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0xe9df0000);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x1620feb;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x57310000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MULTU *********************************  
	testName = "multiplication unsigned positive negative - 0xf00000ef * 0x79df0000";
	testID = mips_test_begin_test("MULTU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mult $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x19;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xf00000ef);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x79df0000);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x72411071;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xc7310000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MULTU *********************************  
	testName = "multiplication unsigned negative negative - 0xf00000ef * 0x89df0000";
	testID = mips_test_begin_test("MULTU");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mult $23, $30
	instruction = (0ul << 26) | (23ul << 21) | (30ul << 16) | (0ul << 11) | (0ul << 6) | 0x19;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xf00000ef);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);
	err = mips_cpu_set_register(cpu1, 30, 0x89df0000);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//mflo $4
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (4ul << 11) | (0ul << 6) | 0x12;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 8, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0x81411080;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	err = mips_cpu_get_register(cpu1, 4, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xb7310000;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
		else 
			outcome = false;
	}
	else 
	{
		stringstream toPrint;
		toPrint << "result in LO: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MTHI *********************************  
	testName = "move to special register HI";
	testID = mips_test_begin_test("MTHI");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mthi $23
	instruction = (0ul << 26) | (23ul << 21) | (0ul << 16) | (0ul << 11) | (0ul << 6) | 0x11;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xf00000ef);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	//test result
	//mfhi $3
	instruction = (0ul << 26) | (0ul << 21) | (0ul << 16) | (3ul << 11) | (0ul << 6) | 0x10;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 4, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);


	//step cpu
	err = mips_cpu_step(cpu1);
	if(err != mips_Success)
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}
	
	//test result
	err = mips_cpu_get_register(cpu1, 3, &buffer);
	if(err != mips_Success)
		log_error(logger, testName, "getRegister failed with error: ", err);

	expected = 0xf00000ef;
	if(buffer == expected)
	{
		if(outcome)
			outcome = true;
	}
	else
	{
		stringstream toPrint;
		toPrint << "result in HI: " << hex << buffer << endl;
		toPrint << "expected result: " << hex << expected << endl;
		log_error(logger, testName, toPrint.str(), mips_Success);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* MTLO *********************************  
	testName = "move to special register LO - bad instruction";
	testID = mips_test_begin_test("MTLO");

	outcome = true;
	// set pc to 0
	err = mips_cpu_set_pc(cpu1, 0);
	if(err != mips_Success)
		log_error(logger, testName, "set PC failed with error: ", err);

	//setup instruction in memory
	//mtlo $23
	instruction = (0ul << 26) | (23ul << 21) | (0ul << 16) | (5ul << 11) | (0ul << 6) | 0x13;
	data[0] = (instruction >> 24)&0xff;
	data[1] = (instruction >> 16)&0xff;
	data[2] = (instruction >> 8)&0xff;
	data[3] = (instruction >> 0)&0xff;
	err = mips_mem_write(ram, 0, 4, data);
	if(err != mips_Success)
		log_error(logger, testName, "memWrite failed with error: ", err);

	//setup registers
	err = mips_cpu_set_register(cpu1, 23, 0xf00000ef);
	if(err != mips_Success)
		log_error(logger, testName, "setRegister failed with error: ", err);

	//step cpu
	err = mips_cpu_step(cpu1);
	if(err == mips_ExceptionInvalidInstruction)
		outcome = true;
	else
	{
		log_error(logger, testName, "cpuStep failed with error: ", err);
		outcome = false;
	}

	mips_test_end_test(testID, outcome, NULL);

	//********************************* END OF TESTS *********************************  

	mips_cpu_free(cpu1);
	cpu1 = 0;
	mips_mem_free(ram);
	ram = 0;

	mips_test_end_suite();

	return 0;
}

void log_error(ofstream &logfile, string testName, string message, mips_error err)
{
	logfile << "Test Name: " << testName << endl;
	logfile << message << hex << err << endl;
	logfile << endl;
}

