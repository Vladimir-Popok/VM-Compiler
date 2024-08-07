#pragma once
#include <iostream>
#include <fstream>	

class VMWriter {

	std::ofstream output;

public:

	VMWriter(std::string filename);
	VMWriter() {};

	void writePush(std::string segment, int index);

	void writePop(std::string segment, int index);

	void writeArithmetic(std::string command);

	void writeLabel(std::string label);

	void writeGoto(std::string label);

	void writeIf(std::string label);

	void writeCall(std::string name, int nArgs);

	void writeFunction(std::string name, int nVars);

	void writeReturn();
	
	void close();
};