#pragma once
#include <iostream>
#include <string>


using namespace std;


class Parser {

public:
	enum Type {
		PUSH_POP,
		ARITHMETIC_LOGIC,
		LABEL,
		GOTO,
		IF_GOTO,
		FUNCTION,
		CALL,
		RETURN
	};

private:
	Type n_operation;

	string m_inst = "",
		m_segment = "",
		m_address = "",
		m_operation = "",
		m_var = "";

public:

	Parser(const string& instruction);

	Type operation() { return n_operation; }
	string getInst() { return m_inst; }
	string getSegment() { return m_segment; }
	string getAddress() { return m_address; }
	string getOperation() { return m_operation; }
	string getVariables() { return m_var; }
};