#pragma once
#include <iostream>
#include <string>
#include "Parser.h"

using namespace std;

class CodeWriter {
	static int num_label;
	static int foo_label;
	string m_fileName;
	string m_code;

public:

	CodeWriter(Parser pars, string fileName);

	string getCode() { return m_code + "\n"; }

private:

	string pop(Parser pars);

	string push(Parser pars);

	string getCodeSegment(string segment);

	string template1(string name, string type);

	string template2(string name, string type);

	string template3(string name, string type);

	string label(string name);

	string go_to(string name);

	string if_goto(string name);

	string call(string name, string var);

	string function(string name, string var);

	string returnFoo();
};