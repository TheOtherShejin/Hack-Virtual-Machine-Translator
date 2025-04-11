#include "Parser.h"
#include "CodeWriter.h"
#include "StringProcessor.h"
#include <fstream>
#include <filesystem>
#include <vector>

const std::string FILE_LOCATION = "D:/SHEJIN/Programs/nand2tetris/projects/8/ProgramFlow/FibonacciSeries/FibonacciSeries.vm";

int main() {
	std::vector<std::filesystem::path> inputPaths;
	std::string outputPath;
	if (IsDirectory(FILE_LOCATION)) {
		inputPaths.push_back(FILE_LOCATION + "/Sys.vm");
		std::filesystem::path path(FILE_LOCATION);
		outputPath = path.string() + '/' + path.filename().string() + ".asm";
		for (const auto& entry : std::filesystem::directory_iterator(FILE_LOCATION)) {
			std::filesystem::path path = entry.path();
			if (path.extension().string() != ".vm" || path.filename().string() == "Sys.vm") continue;
			inputPaths.push_back(path);
		}
	}
	else {
		inputPaths.push_back(std::filesystem::path(FILE_LOCATION));
		outputPath = std::filesystem::path(FILE_LOCATION).replace_extension(std::filesystem::path(".asm")).string();
	}

	std::ofstream outputFile(outputPath);
	std::cout << "Translating .vm File(s) at " << FILE_LOCATION << " to .asm File.\n";

	Parser parser;
	CodeWriter writer;

	outputFile << "// Bootstrap Code\n";
	outputFile << writer.WriteInit();

	for (auto& path : inputPaths) {
		std::ifstream inputFile(path.string());
		if (!inputFile.is_open()) {
			std::cerr << "Failed to Open Input File at: " << FILE_LOCATION << '\n';
			return -1;
		}

		writer.fileName = path.filename().string();

		std::string line;
		while (std::getline(inputFile, line)) {
			line = Trim(line);
			if (line[0] == '/' || line == "") continue;

			std::string comment = "// " + line + '\n';
			outputFile << comment;

			Instruction instruction = parser.Parse(line);
			std::string code = writer.WriteCode(instruction);

			outputFile << code;
		}

		inputFile.close();
	}

	outputFile.close();
	std::cout << "Translation Complete! .asm File is Saved at: " << outputPath << '\n';


	return 0;
}