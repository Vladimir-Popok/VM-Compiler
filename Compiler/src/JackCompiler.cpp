#include "../include/JackCompiler.h"

std::string relative_path(const std::string& fullPath) {
	size_t slash = fullPath.rfind('/');
	if (slash != std::string::npos)
		return fullPath.substr(slash + 1, fullPath.size() - (slash + 1));
	size_t backslash = fullPath.rfind('\\');
	if (backslash != std::string::npos)
		return fullPath.substr(backslash + 1, fullPath.size() - (backslash + 1));
	return fullPath;
}



int compile(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Usage: program_name file_or_folder_path output_file" << std::endl;
		return 1;
	}
	std::string arg0 = argv[0];
	std::string  path = argv[1];
	std::string  arg1 = relative_path(argv[1]);

	size_t lastindex = arg1.find_last_of(".");
	std::string inputFile = arg1.substr(0, lastindex);

	std::cout << inputFile << std::endl;
	if (fs::is_directory(path)) {
		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.is_regular_file() && entry.path().extension() == ".jack") {
				std::string filename = entry.path().string();
				size_t lastindex = filename.find_last_of(".");
				filename = filename.substr(0, lastindex);
				Tokenizer token = Tokenizer(filename);
				CompilationEngine comp = CompilationEngine(token, filename);
				comp.compileClass();

			}
		}
	}
	else {
		std::string filename = argv[1];
		size_t lastindex = filename.find_last_of(".");
		filename = filename.substr(0, lastindex);
		Tokenizer token = Tokenizer(filename);
		CompilationEngine comp = CompilationEngine(token, filename);
		comp.compileClass();

	}

	return 0;
}


