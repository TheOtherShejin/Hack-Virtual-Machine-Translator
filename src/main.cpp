#include "Parser.h"
#include "CodeWriter.h"
#include <fstream>

const std::string FILE_LOCATION = "D:/SHEJIN/Programs/nand2tetris/projects/7/MemoryAccess/StaticTest/StaticTest.vm";

std::string Trim(std::string text) {
	int strBegin = text.find_first_not_of(' ');
	if (strBegin == std::string::npos) return "";

	int strEnd = text.find_last_not_of(' ');
	int strRange = strEnd - strBegin + 1;
	return text.substr(strBegin, strRange);
}

std::string GetFileName(std::string path) {
	int nameStart = path.find_last_of('/')+1;
	int nameEnd = path.find_last_of('.')-1;
	int nameRange = nameEnd - nameStart + 1;
	return path.substr(nameStart, nameRange);
}

int main() {
	std::ifstream inputFile(FILE_LOCATION);
	std::string outputPath = FILE_LOCATION.substr(0, FILE_LOCATION.find_last_of('.') + 1) + "asm";
	std::ofstream outputFile(outputPath);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to Open Input File at: " << FILE_LOCATION << '\n';
		return -1;
	}

	std::cout << "Translating .vm File at " << FILE_LOCATION << " to .asm File.\n";

	Parser parser;
	CodeWriter writer;
	writer.fileName = GetFileName(FILE_LOCATION);

	std::string line;
	while (std::getline(inputFile, line)) {
		line = Trim(line);
		if (line[0] == '/' || line == "") continue;

		std::string comment = "// " + line + '\n';
		outputFile << comment;

		Instruction instruction = parser.Parse(line);
		std::string code = instruction.arithmetic ? writer.WriteArithmetic(instruction) : writer.WritePushPop(instruction);

		outputFile << code;
	}

	inputFile.close();
	outputFile.close();

	std::cout << "Translation Complete! .asm File is Saved at: " << outputPath << '\n';

	return 0;
}