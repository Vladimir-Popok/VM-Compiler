#include "../include/Parser.h"

Parser::Parser(const string& instruction) {
	if (instruction.find("push") != string::npos || instruction.find("pop") != string::npos) {
		int i = 0;
		while (instruction[i] != ' ' && i < instruction.length())
			m_inst += instruction[i++];
		++i;
		while (instruction[i] != ' ' && i < instruction.length())
			m_segment += instruction[i++];
		++i;
		while (instruction[i] != ' ' && i < instruction.length())
			m_address += instruction[i++];

		n_operation = PUSH_POP;
	}

	else if (instruction.find("label") != string::npos) {
		
		string substr = instruction.substr(instruction.find(" "), instruction.length());
		for (int i = 1; substr[i] != ' ' && i < substr.length(); ++i)
			m_inst += substr[i];
		
		n_operation = LABEL;
	}

	else if (instruction.find("goto") != string::npos) {

		string substr = instruction.substr(instruction.find(" "), instruction.length());
		for (int i = 1; substr[i] != ' ' && i < substr.length(); ++ i)
			m_inst += substr[i];

		if (instruction.find("if-goto") != string::npos) 
			n_operation = IF_GOTO;
		
		else
			n_operation = GOTO;
	}

	else if (instruction.find("function") != string::npos || instruction.find("call") != string::npos) {
		
		int i = 1;
		string substr = instruction.substr(instruction.find(" "), instruction.length());
		for (; substr[i] != ' ' && i < substr.length(); ++i)
			m_inst += substr[i];

		for (++i; substr[i] != ' ' && i < substr.length(); ++i)
			m_var += substr[i];
		
		if (instruction.find("function") != string::npos)
			n_operation = FUNCTION;
		else
			n_operation = CALL;
	}

	else if (instruction.find("return") != string::npos) {
		n_operation = RETURN;
	}

	else {
		for (int i = 0; instruction[i] != ' ' && i < instruction.length(); ++i)
			m_operation += instruction[i];

		n_operation = ARITHMETIC_LOGIC;
	}
}