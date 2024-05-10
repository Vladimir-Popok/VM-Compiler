#include "JackCompiler.h"
#include ""

int main(int argc, char** argv) {
	if (compile(argc, argv) != 0) {
		std::cerr << "Compilation failed." << std::endl;
		return 1;
	}

	if (translate(argc, argv) != 0) {
		std::cerr << "Translation failed." << std::endl;
		return 1;
	}

	return 0;
}