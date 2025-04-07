#include "CodeWriter.h"

std::string CodeWriter::WriteArithmetic(Instruction instruction) {
	if (instruction.opcode == "add") {
		lineNum += 8;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D+M\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "sub") {
		lineNum += 8;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M-D\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "and") {
		lineNum += 8;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D&M\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "or") {
		lineNum += 8;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D|M\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "neg") {
		lineNum += 5;
		return "@SP\nAM=M-1\nM=-M\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "not") {
		lineNum += 5;
		return "@SP\nAM=M-1\nM=!M\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "eq") {
		lineNum += 18;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@" + std::to_string(lineNum - 5) + "\nD;JEQ\n@SP\nA=M\nM=0\n@" + std::to_string(lineNum - 2) + "\n0;JMP\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "lt") {
		lineNum += 18;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@" + std::to_string(lineNum - 5) + "\nD;JLT\n@SP\nA=M\nM=0\n@" + std::to_string(lineNum - 2) + "\n0;JMP\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n";
	}
	else if (instruction.opcode == "gt") {
		lineNum += 18;
		return "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@" + std::to_string(lineNum - 5) + "\nD;JGT\n@SP\nA=M\nM=0\n@" + std::to_string(lineNum - 2) + "\n0;JMP\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n";
	}
	else {
		std::cerr << "Arithmetic Instruction is Invalid. Instruction: " << instruction.opcode << '\n';
		return "";
	}
}

std::string CodeWriter::WritePushPop(Instruction instruction) {
	if (instruction.opcode == "push") {
		if (instruction.arg1 == "constant") {
			lineNum += 7;
			return "@" + instruction.arg2 + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "static") {
			lineNum += 7;
			return "@" + fileName + "." + instruction.arg2 + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "temp") {
			lineNum += 7;
			return "@" + std::to_string(5 + std::stoi(instruction.arg2)) + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (instruction.arg1 == "pointer") {
			lineNum += 7;
			return "@" + std::to_string(3 + std::stoi(instruction.arg2)) + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else {
			lineNum += 10;
			std::string segment = segmentMap.at(instruction.arg1);
			return "@" + segment + "\nD=M\n@" + instruction.arg2 + "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
	}
	else if (instruction.opcode == "pop") {
		if (instruction.arg1 == "static") {
			lineNum += 5;
			return "@SP\nAM=M-1\nD=M\n@" + fileName + "." + instruction.arg2 + "\nM=D\n";
		}
		else if (instruction.arg1 == "temp") {
			lineNum += 5;
			return "@SP\nAM=M-1\nD=M\n@" + std::to_string(5 + std::stoi(instruction.arg2)) + "\nM=D\n";
		}
		else if (instruction.arg1 == "pointer") {
			lineNum += 5;
			return "@SP\nAM=M-1\nD=M\n@" + std::to_string(3 + std::stoi(instruction.arg2)) + "\nM=D\n";
		}
		else {
			lineNum += 12;
			std::string segment = segmentMap.at(instruction.arg1);
			return "@" + segment + "\nD=M\n@" + instruction.arg2 + "\nD=D+A\n@R15\nM=D\n@SP\nAM=M-1\nD=M\n@R15\nA=M\nM=D\n";
		}
	}
	else {
		std::cerr << "Push/Pop Instruction is Invalid. Instruction: " << instruction.opcode << ' ' << instruction.arg1 << ' ' << instruction.arg2 << '\n';
		return "";
	}
}