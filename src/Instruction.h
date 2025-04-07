#pragma once

#include <string>

struct Instruction {
	bool arithmetic = false;
	std::string opcode, arg1, arg2;
};