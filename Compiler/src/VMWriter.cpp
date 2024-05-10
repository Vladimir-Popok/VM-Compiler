#include "../include/VMWriter.h"

VMWriter::VMWriter(std::string filename) {
	
	output.open(filename + ".vm", std::ios::trunc);
	if (!output) {
		output.close();
		std::cerr << "Error: Could not open file " << filename << std::endl;
		exit(1);
	}
}

void VMWriter::writePush(std::string segment, int index) {
	output << "push " << segment << " " << index << std::endl;
}

void VMWriter::writePop(std::string segment, int index) {
	output << "pop " << segment << " " << index << std::endl;
}

void VMWriter::writeArithmetic(std::string command) {
	if (command == "+") output << "add" << std::endl;
	else if (command == "-") output << "sub" << std::endl;
	else if (command == "*") writeCall("Math.multiply", 2);
	else if (command == "/") writeCall("Math.divide", 2);
	else if (command == "&amp;") output << "and" << std::endl;
	else if (command == "|") output << "or" << std::endl;
	else if (command == "&lt;") output << "lt" << std::endl;
	else if (command == "&gt;") output << "gt" << std::endl;
	else if (command == "=") output << "eq" << std::endl;
	else output << command << std::endl;
}

void VMWriter::writeLabel(std::string label) {
	output << "label " << label << std::endl;
}

void VMWriter::writeGoto(std::string label) {
	output << "goto " << label << std::endl;
}

void VMWriter::writeIf(std::string label) {
	output << "if-goto " << label << std::endl;
}

void VMWriter::writeCall(std::string name, int nArgs) {
	output << "call " << name << " " << nArgs << std::endl;
}

void VMWriter::writeFunction(std::string name, int nVars) {
	output << "function " << name << " " << nVars << std::endl;
}

void VMWriter::writeReturn() {
	output << "return" << std::endl;
}

void VMWriter::close() {
	output.close();
}
