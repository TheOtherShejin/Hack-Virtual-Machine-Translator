#include "CodeWriter.h"

std::string CodeWriter::WriteInit() {
	Instruction instruction;
	instruction.arg1 = "Sys.init";
	instruction.arg2 = "0";
	return "@256\nD=A\n@SP\nM=D\n" + WriteCall(instruction);
}

std::string CodeWriter::WriteCode(Instruction instruction) {
	switch (instruction.type)
	{
	case ARITHMETIC:
		return WriteArithmetic(instruction);
	case PUSH_POP:
		return WritePushPop(instruction);
	case LABEL:
		return WriteLabel(instruction);
	case GOTO:
		return WriteGoto(instruction);
	case IF_GOTO:
		return WriteIf(instruction);
	case CALL:
		return WriteCall(instruction);
	case FUNCTION:
		return WriteFunction(instruction);
	case RETURN:
		return WriteReturn(instruction);
	}
}

std::string CodeWriter::WriteArithmetic(Instruction instruction) {
	if (instruction.opcode == "add") {
		return "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n";
	}
	else if (instruction.opcode == "sub") {
		return "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n";
	}
	else if (instruction.opcode == "and") {
		return "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n";
	}
	else if (instruction.opcode == "or") {
		return "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n";
	}
	else if (instruction.opcode == "neg") {
		return "@SP\nA=M-1\nM=-M\n";
	}
	else if (instruction.opcode == "not") {
		return "@SP\nA=M-1\nM=!M\n";
	}
	else if (instruction.opcode == "eq") {
		arJumpIndex++;
		std::string jumpIndexStr = std::to_string(arJumpIndex);
		return "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@ARITHMETIC_TRUE_" + jumpIndexStr + "\nD;JEQ\n@SP\nA=M-1\nM=0\n@ARITHMETIC_END_" + jumpIndexStr + "\n0;JMP\n(ARITHMETIC_TRUE_" + jumpIndexStr + ")\n@SP\nA=M-1\nM=-1\n(ARITHMETIC_END_" + jumpIndexStr + ")\n";
	}
	else if (instruction.opcode == "lt") {
		arJumpIndex++;
		std::string jumpIndexStr = std::to_string(arJumpIndex);
		return "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@ARITHMETIC_TRUE_" + jumpIndexStr + "\nD;JLT\n@SP\nA=M-1\nM=0\n@ARITHMETIC_END_" + jumpIndexStr + "\n0;JMP\n(ARITHMETIC_TRUE_" + jumpIndexStr + ")\n@SP\nA=M-1\nM=-1\n(ARITHMETIC_END_" + jumpIndexStr + ")\n";
	}
	else if (instruction.opcode == "gt") {
		arJumpIndex++;
		std::string jumpIndexStr = std::to_string(arJumpIndex);
		return "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@ARITHMETIC_TRUE_" + jumpIndexStr + "\nD;JGT\n@SP\nA=M-1\nM=0\n@ARITHMETIC_END_" + jumpIndexStr + "\n0;JMP\n(ARITHMETIC_TRUE_" + jumpIndexStr + ")\n@SP\nA=M-1\nM=-1\n(ARITHMETIC_END_" + jumpIndexStr + ")\n";
	}
	else {
		std::cerr << "Arithmetic Instruction is Invalid. Instruction: " << instruction.opcode << ' ' << instruction.type << '\n';
		return "";
	}
}

std::string CodeWriter::WritePushPop(Instruction instruction) {
	if (instruction.opcode == "push") {
		if (instruction.arg1 == "constant") {
			return "@" + instruction.arg2 + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "static") {
			return "@" + fileName + "." + instruction.arg2 + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "temp") {
			return "@" + std::to_string(5 + std::stoi(instruction.arg2)) + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "pointer") {
			return "@" + std::to_string(3 + std::stoi(instruction.arg2)) + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else {
			std::string segment = segmentMap.at(instruction.arg1);
			return "@" + segment + "\nD=M\n@" + instruction.arg2 + "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
	}
	else if (instruction.opcode == "pop") {
		if (instruction.arg1 == "static") {
			return "@SP\nAM=M-1\nD=M\n@" + fileName + "." + instruction.arg2 + "\nM=D\n";
		}
		else if (instruction.arg1 == "temp") {
			return "@SP\nAM=M-1\nD=M\n@" + std::to_string(5 + std::stoi(instruction.arg2)) + "\nM=D\n";
		}
		else if (instruction.arg1 == "pointer") {
			return "@SP\nAM=M-1\nD=M\n@" + std::to_string(3 + std::stoi(instruction.arg2)) + "\nM=D\n";
		}
		else {
			std::string segment = segmentMap.at(instruction.arg1);
			return "@" + segment + "\nD=M\n@" + instruction.arg2 + "\nD=D+A\n@R15\nM=D\n@SP\nAM=M-1\nD=M\n@R15\nA=M\nM=D\n";
		}
	}
	else {
		std::cerr << "Push/Pop Instruction is Invalid. Instruction: " << instruction.opcode << ' ' << instruction.arg1 << ' ' << instruction.arg2 << '\n';
		return "";
	}
}

std::string CodeWriter::WriteLabel(Instruction instruction) {
	return "(" + instruction.arg1 + ")\n";
}

std::string CodeWriter::WriteGoto(Instruction instruction) {
	return "@" + instruction.arg1 + "\n0;JMP";
}

std::string CodeWriter::WriteIf(Instruction instruction) {
	return "@SP\nAM=M-1\nD=M\n@" + instruction.arg1 + "\nD;JNE\n";
}

std::string CodeWriter::WriteCall(Instruction instruction) {
	returnIndex++;
	std::string code = "@" + instruction.arg1 + "$ret." + std::to_string(returnIndex) + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	code += "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	code += "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	code += "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	code += "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	code += "@" + std::to_string(5+std::stoi(instruction.arg2)) + "\nD=A\n@SP\nD=M-D\n@ARG\nM=D\n";
	code += "@" + instruction.arg1 + "\n0;JMP\n";
	code += "(" + instruction.arg1 + "$ret." + std::to_string(returnIndex) + ")\n";
	return code;
}

std::string CodeWriter::WriteFunction(Instruction instruction) {
	std::string code = "(" + instruction.arg1 + ")" + "\n@SP\nD=M\n@LCL\nM=D\n@" + instruction.arg2 + "\nD=A\n@SP\nM=M+D\n";
	int nVars = std::stoi(instruction.arg2);
	if (nVars > 0) {
		code += "@LCL\nA=M\nM=0\n";
		for (int i = 1; i < nVars; i++) code += "A=A+1\nM=0\n";
	}
	return code;
}

std::string CodeWriter::WriteReturn(Instruction instruction) {
	std::string code = "@LCL\nD=M\n@5\nA=D-A\nD=M\n@R15\nM=D\n";
	code += "@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n";
	code += "@ARG\nD=M\n@SP\nM=D+1\n";
	code += "@LCL\nAM=M-1\nD=M\n@THAT\nM=D\n";
	code += "@LCL\nAM=M-1\nD=M\n@THIS\nM=D\n";
	code += "@LCL\nAM=M-1\nD=M\n@ARG\nM=D\n";
	code += "@LCL\nAM=M-1\nD=M\n@LCL\nM=D\n";
	code += "@R15\nA=M\n0;JMP\n";
	return code;
}

void CodeWriter::ResetCounters() {
	arJumpIndex = -1;
	returnIndex = -1;
}