#pragma once

/*
	// PUSH SEGMENT			// PUSH CONSTANT	// PUSH STATIC		// PUSH TEMP	// PUSH POINTER
	*SP = *(segment + i)	*SP = constant		*SP = *Foo.i		*SP = *(5 + i)	*SP = *(3 + i)
	SP++					SP++				SP++				SP++			SP++

	@segment
	D=M
	@i
	A=D+A					@constant			@Foo.i				@5+i			@3+i
	D=M						D=A					D=M					D=M				D=M
	@SP						@SP					@SP					@SP				@SP
	A=M						A=M					A=M					A=M				A=M
	M=D						M=D					M=D					M=D				M=D
	@SP						@SP					@SP					@SP				@SP
	M=M+1					M=M+1				M=M+1				M=M+1			M=M+1

	// POP SEGMENT			// POP STATIC		// POP TEMP			// POP POINTER
	R15 = segment + i
	SP--					SP--				SP--				SP--
	*R15 = *SP				*Foo.i = *SP		*(5+i) = *SP		*(3+i) = *SP

	@segment
	D=M
	@i
	D=D+A
	@R15
	M=D
	@SP						@SP					@SP					@SP
	AM=M-1					AM=M-1				AM=M-1				AM=M-1
	D=M						D=M					D=M					D=M
	@R15					@Foo.i				@5+i				@3+i
	A=M						M=D					M=D					M=D
	M=D
*/

/*
	// ADD		// SUB		// AND		// OR
	SP--		SP--		SP--		SP--
	D=*SP		D=*SP		D=*SP		D=*SP
	SP--		SP--		SP--		SP--
	M=*SP		M=*SP		M=*SP		M=*SP
	M=D+M		M=M-D		M=D&M		M=D|M
	SP++		SP++		SP++		SP++

	// NEG		// NOT
	SP--		SP--
	M=*SP		M=*SP
	M=-M		M=!M
	SP++		SP++

	// EQ		// LT		// GT
	SP--		SP--		SP--
	D=*SP		D=*SP		D=*SP
	SP--		SP--		SP--
	M=*SP		M=*SP		M=*SP
	D=M-D		D=M-D		D=M-D
	@LABEL_i	@LABEL_i	@LABEL_i
	D;JEQ		D;JLT		D;JGT
	*SP=0		*SP=0		*SP=0
	@END_i		@END_i		@END_i
	0;JMP		0;JMP		0;JMP
	(LABEL_i)	(LABEL_i)	(LABEL_i)	// (ARITHMETIC_TRUE_i)
	*SP=1		*SP=1		*SP=1
	(END_i)		(END_i)		(END_i)		// (ARITHMETIC_END_i)
	SP++		SP++		SP++

-------------------------------------------------

	// ADD		// SUB		// AND		// OR
	@SP			@SP			@SP			@SP
	AM=M-1		AM=M-1		AM=M-1		AM=M-1
	D=M			D=M			D=M			D=M
	@SP			@SP			@SP			@SP
	AM=M-1		AM=M-1		AM=M-1		AM=M-1
	M=D+M		M=M-D		M=D&M		M=D|M
	@SP			@SP			@SP			@SP
	M=M+1		M=M+1		M=M+1		M=M+1

	// NEG		// NOT
	@SP			@SP
	AM=M-1		AM=M-1
	M=-M		M=!M
	@SP			@SP
	M=M+1		M=M+1

	// EQ		// LT		// GT
	@SP			@SP			@SP
	AM=M-1		AM=M-1		AM=M-1
	D=M			D=M			D=M
	@SP			@SP			@SP
	AM=M-1		AM=M-1		AM=M-1
	D=M-D		D=M-D		D=M-D
	@LABEL_i	@LABEL_i	@LABEL_i
	D;JEQ		D;JLT		D;JGT
	@SP			@SP			@SP
	A=M			A=M			A=M
	M=0			M=0			M=0
	@END_i		@END_i		@END_i
	0;JMP		0;JMP		0;JMP
	(LABEL_i)	(LABEL_i)	(LABEL_i)
	@SP			@SP			@SP
	A=M			A=M			A=M
	M=-1		M=-1		M=-1
	(END_i)		(END_i)		(END_i)
	@SP			@SP			@SP
	M=M+1		M=M+1		M=M+1
*/

#include "Instruction.h"
#include <unordered_map>
#include <iostream>

class CodeWriter {
private:
	std::unordered_map<std::string, std::string> segmentMap = {
		{ "argument", "ARG" },
		{ "local", "LCL" },
		{ "this", "THIS" },
		{ "that", "THAT" },
		{ "pointer", "THIS" },
	};
	int arJumpIndex = -1;
	int returnIndex = -1;
public:
	std::string fileName = "";

	std::string WriteInit();
	std::string WriteCode(Instruction instruction);
	std::string WriteArithmetic(Instruction instruction);
	std::string WritePushPop(Instruction instruction);
	std::string WriteLabel(Instruction instruction);
	std::string WriteGoto(Instruction instruction);
	std::string WriteIf(Instruction instruction);
	std::string WriteCall(Instruction instruction);
	std::string WriteFunction(Instruction instruction);
	std::string WriteReturn(Instruction instruction);
	void ResetCounters();
};