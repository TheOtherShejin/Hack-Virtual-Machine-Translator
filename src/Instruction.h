#pragma once

#include <string>

enum OpcodeType {
	ARITHMETIC,
	PUSH_POP,
	LABEL,
	GOTO,
	IF_GOTO,
	CALL,
	FUNCTION,
	RETURN
};

struct Instruction {
	OpcodeType type;
	std::string opcode, arg1, arg2;
};