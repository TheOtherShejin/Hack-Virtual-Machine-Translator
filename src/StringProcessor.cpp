#include "StringProcessor.h"

bool IsDirectory(std::string path) {
	return path.find_last_of('.') == std::string::npos;
}

std::string Trim(std::string text) {
	int firstNotSpace = text.find_first_not_of(' ');
	int firstNotTab = text.find_first_not_of('\t');
	int strBegin = fmax(firstNotSpace == std::string::npos ? -1 : firstNotSpace, firstNotTab == std::string::npos ? -1 : firstNotTab);
	if (strBegin == -1) return "";

	int strEnd = text.find_last_not_of(' ');
	int strRange = strEnd - strBegin + 1;
	return text.substr(strBegin, strRange);
}