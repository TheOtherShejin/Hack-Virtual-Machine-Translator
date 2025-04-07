#include "Parser.h"

std::vector<std::string> Parser::Tokenize(std::string text, char delim) const {
	std::vector<std::string> tokens;
	std::stringstream iss(text);
	std::string token;
	while (std::getline(iss, token, delim)) {
		tokens.push_back(token);
	}
	return tokens;
}

Instruction Parser::Parse(std::string code) const {
	Instruction instruction;
	std::vector<std::string> tokens = Tokenize(code, ' ');

	instruction.arithmetic = !(tokens[0] == "push" || tokens[0] == "pop");
	instruction.opcode = tokens[0];
	instruction.arg1 = tokens.size() == 3 ? tokens[1] : "";
	instruction.arg2 = tokens.size() == 3 ? tokens[2] : "";

	return instruction;
}