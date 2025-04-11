#include "Parser.h"

std::vector<std::string> Parser::Tokenize(std::string text, char delim) const {
	std::vector<std::string> tokens;
	std::stringstream iss(text);
	std::string token;
	while (std::getline(iss, token, delim)) {
		if (tokens.size() == 3) break;
		tokens.push_back(token);
	}
	tokens[tokens.size() - 1] = Trim(tokens[tokens.size() - 1]);
	return tokens;
}

Instruction Parser::Parse(std::string code) const {
	Instruction instruction;
	std::vector<std::string> tokens = Tokenize(code, ' ');

	instruction.opcode = tokens[0];

	if (instruction.opcode == "push" || instruction.opcode == "pop") instruction.type = PUSH_POP;
	else if (instruction.opcode == "label") instruction.type = LABEL;
	else if (instruction.opcode == "goto") instruction.type = GOTO;
	else if (instruction.opcode == "if-goto") instruction.type = IF_GOTO;
	else if (instruction.opcode == "call") instruction.type = CALL;
	else if (instruction.opcode == "function") instruction.type = FUNCTION;
	else if (instruction.opcode == "return") instruction.type = RETURN;
	else instruction.type = ARITHMETIC;

	instruction.arg1 = tokens.size() >= 2 ? tokens[1] : "";
	instruction.arg2 = tokens.size() == 3 ? tokens[2] : "";

	return instruction;
}