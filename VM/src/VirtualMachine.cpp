#include "../include/VirtualMachine.h"

bool isWhitespace(string s) {
	for (int index = 0; index < s.length(); index++) {
		if (!std::isspace(s[index]))
			return false;
	}
	return true;
}

string relative_path(const string& fullPath) {
	size_t slash = fullPath.rfind('/');
	if (slash != string::npos)
		return fullPath.substr(slash + 1, fullPath.size() - (slash + 1));
	size_t backslash = fullPath.rfind('\\');
	if (backslash != string::npos)
		return fullPath.substr(backslash + 1, fullPath.size() - (backslash + 1));
	return fullPath;
}

string initialization() {
	string code = "";
	code.append("@256\n");
	code.append("D=A\n");
	code.append("@SP\n");
	code.append("M=D\n");

	code.append("// call Sys.init 0\n");
	code.append("@PROGRAM_END\n");
	code.append("D=A\n");
	code.append("@SP\n");
	code.append("A=M\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("M=M+1\n");

	code.append("@LCL\n");
	code.append("D=M\n");
	code.append("@SP\n");
	code.append("A=M\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("M=M+1\n");

	code.append("@ARG\n");
	code.append("D=M\n");
	code.append("@SP\n");
	code.append("A=M\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("M=M+1\n");

	code.append("@THIS\n");
	code.append("D=M\n");
	code.append("@SP\n");
	code.append("A=M\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("M=M+1\n");

	code.append("@THAT\n");
	code.append("D=M\n");
	code.append("@SP\n");
	code.append("A=M\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("M=M+1\n");

	code.append("@5\n");
	code.append("D=A\n");
	code.append("@SP\n");
	code.append("D=M-D\n");
	code.append("@0\n");
	code.append("D=D-A\n");
	code.append("@ARG\n");
	code.append("M=D\n");

	code.append("@SP\n");
	code.append("D=M\n");
	code.append("@LCL\n");
	code.append("M=D\n");

	code.append("@Sys.init\n");
	code.append("0;JMP\n");
	code.append("(PROGRAM_END)\n");

	return code;
}

int translate(int argc, char** argv)
{
	if (argc != 2) {
		std::cout << "Error" << endl;
		return 1;
	}
	string arg0 = argv[0];
	string path = argv[1];
	string arg1 = relative_path(argv[1]);
	
	size_t lastindex = arg1.find_last_of(".");
	string inputFile = arg1.substr(0, lastindex);
	
	std::cout << inputFile << endl;
	ofstream output(inputFile + ".asm", ios::trunc);
	if (!output) {
		std::cout << "Failed to create output file: " << inputFile + ".asm" << std::endl;
		return 2;
	}
	
	if (fs::is_directory(path)) {
		if (fs::exists(path + "/Sys.vm"))
			output << initialization();

		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.is_regular_file() && entry.path().extension() == ".vm") {
				ifstream input(entry.path());

				if (!input) {
					std::cout << "Failed to open file: " << entry.path() << endl;
					continue;
				}
				
				string filename = fs::path(entry).stem().string();
				std::cout << filename << endl;
				string instruction;
				while (getline(input, instruction)) {
					if (isWhitespace(instruction))
						continue;
					if (instruction.find("//") != string::npos)
						instruction = instruction.substr(0, instruction.find("//"));

					Parser pars = Parser(instruction);
					CodeWriter writer = CodeWriter(pars, filename);
					output << writer.getCode();
				}
				input.close();
			}
		}	
	}
		
	else {
		ifstream input(argv[1]);
		if (!input) {
			std::cout << "Failed to open file: " << path << endl;
		}

		string instruction;
		while (getline(input, instruction)) {
			if (isWhitespace(instruction))
				continue;
			if (instruction.find("//") != string::npos)
				instruction = instruction.substr(0, instruction.find("//"));

			Parser pars = Parser(instruction);
			CodeWriter writer = CodeWriter(pars, inputFile);
			output << writer.getCode();
		}
	}
	output.close();
	std::cout << "\nComplete" << endl;
	
	return 0;
}


