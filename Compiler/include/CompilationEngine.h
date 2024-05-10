#pragma once
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"
#include <string>

class CompilationEngine {
	
	std::string m_filename;
	std::string class_name;
	int count_label = 0;
	
	struct Data {
		std::string data_type;
		std::string type_foo;
	};

	std::unordered_map <std::string, Data> foo_table;
	Tokenizer m_tokenizer;
	SymbolTable sym_tab;
	VMWriter writer;

public:
	CompilationEngine(const Tokenizer& tokenizer, std::string filename);

	void compileClass();


private:

	void compileClassVarDec();

	void compileSubroutine();

	void compileParameterList();
	
	void compileSubroutineBody();

	void compileVarDec();

	void compileStatements();

	void compileLet();

	void compileIf();

	void compileWhile();

	void compileDo();

	void compileReturn();

	void compileExpression();
	
	void compileTerm();
	
	void errorMesage(std::string type, std::string expected, int error_code, std::string met = "");
};
