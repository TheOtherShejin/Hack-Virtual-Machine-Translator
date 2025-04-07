#pragma once

#include "Instruction.h"
#include <iostream>
#include <sstream>
#include <vector>

class Parser {
private:
	std::vector<std::string> Tokenize(std::string text, char delim) const;
public:
	Instruction Parse(std::string code) const;
};