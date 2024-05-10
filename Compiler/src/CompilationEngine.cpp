#include "../include/CompilationEngine.h"

CompilationEngine::CompilationEngine(const Tokenizer& tokenizer, std::string filename) : m_tokenizer(tokenizer) {
	m_filename = filename;
	writer = VMWriter(filename);
}

void CompilationEngine::compileClass() {

	m_tokenizer.advance();
	if (m_tokenizer.keyWord() != "class") errorMesage("SyntaxError", "class", 2);
	
	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
	class_name = m_tokenizer.identifier();
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);

	m_tokenizer.advance();
	while (m_tokenizer.hasMoreTokens() && m_tokenizer.tokenType() == "keyword"
		&& (m_tokenizer.keyWord() == "static" || m_tokenizer.keyWord() == "field"
		 || m_tokenizer.keyWord() == "constructor" || m_tokenizer.keyWord() == "function"
		 || m_tokenizer.keyWord() == "method" )) {
		if (m_tokenizer.keyWord() == "static" || m_tokenizer.keyWord() == "field")
			compileClassVarDec();
		else compileSubroutine();
		m_tokenizer.advance();
	}
	if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "class", 2);
	
	writer.close();
}

void CompilationEngine::compileClassVarDec() {
	std::string kind = m_tokenizer.keyWord();
	if (kind == "field") kind = "this";

	m_tokenizer.advance();
	if(m_tokenizer.keyWord() != "char" && m_tokenizer.keyWord() != "boolean" && m_tokenizer.keyWord() != "int" && m_tokenizer.keyWord() != "string" && m_tokenizer.identifier() == "None") 
		errorMesage("SyntaxError", "variable type", 2);

	std::string type;
	if (m_tokenizer.identifier() != "None") type = m_tokenizer.identifier();

	else type = m_tokenizer.keyWord();

	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);

	std::string name = m_tokenizer.identifier();
	sym_tab.defineClass(name, type, kind);

	m_tokenizer.advance();
	while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ";") {

		if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
		
		m_tokenizer.advance();
		if (m_tokenizer.tokenType() != "identifier") errorMesage("SyntaxError", "identifier", 2);

		name = m_tokenizer.identifier();
		sym_tab.defineClass(name, type, kind);

		m_tokenizer.advance();
	}
	if (m_tokenizer.symbol() != ";") errorMesage("SyntaxError", ";", 2);
}

void CompilationEngine::compileSubroutine() {
	
	sym_tab.reset();
	std::string type_foo = m_tokenizer.keyWord();
	m_tokenizer.advance();
	
	if (m_tokenizer.keyWord() != "void" && m_tokenizer.keyWord() != "char" && m_tokenizer.keyWord() != "boolean" && m_tokenizer.keyWord() != "int" && m_tokenizer.keyWord() != "string" && m_tokenizer.identifier() == "None")
		errorMesage("SyntaxError", "variable type", 2);
	std::string data_type = m_tokenizer.keyWord();

	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
	std::string foo_name = m_tokenizer.identifier();
	foo_table[foo_name] = Data{ data_type, type_foo };

	if (type_foo == "method") sym_tab.defineSub("this", "", "argument");

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "(") errorMesage("SyntaxError", "(", 2);

	compileParameterList();

	if (m_tokenizer.symbol() != ")") errorMesage("SyntaxError", ")", 2);

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);

	m_tokenizer.advance();
	while (m_tokenizer.hasMoreTokens() && m_tokenizer.keyWord() == "var") {
		compileVarDec();
		m_tokenizer.advance();
	}

	writer.writeFunction(class_name + "." + foo_name, sym_tab.varCount("local"));
	if (type_foo == "constructor") {
		if (sym_tab.varCountC("this") > 0) {
			writer.writePush("constant", sym_tab.varCountC("this"));
			writer.writeCall("Memory.alloc", 1);
		}
		writer.writePop("pointer", 0);
	}
	else if (type_foo == "method") {
		writer.writePush("argument", 0);
		writer.writePop("pointer", 0);
	}

	compileStatements();
	
	if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "}", 2);

}

void CompilationEngine::compileParameterList() {
	
	m_tokenizer.advance();

	while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ")") {
		if (m_tokenizer.keyWord() != "char" && m_tokenizer.keyWord() != "boolean" && m_tokenizer.keyWord() != "int" && m_tokenizer.keyWord() != "string" && m_tokenizer.identifier() == "None") 
			errorMesage("SyntaxError", "variable type", 2);

		std::string type;
		if (m_tokenizer.identifier() != "None") {
	
			type = m_tokenizer.identifier();
		}
		else {

			type = m_tokenizer.keyWord();
		}
			m_tokenizer.advance();

		if (m_tokenizer.tokenType() != "identifier") errorMesage("SyntaxError", "identifier", 2);

		
		std::string name = m_tokenizer.identifier();
		sym_tab.defineSub(name, type, "argument");

		m_tokenizer.advance();
		if (m_tokenizer.symbol() == ")") break;
		if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
		m_tokenizer.advance();
	}
	
}

void CompilationEngine::compileSubroutineBody() {
	
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);

	m_tokenizer.advance();
	while (m_tokenizer.hasMoreTokens() && m_tokenizer.keyWord() == "var") {
		compileVarDec();
		m_tokenizer.advance();
	}
	
	compileStatements();
	
	if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "}", 2);
	
}

void CompilationEngine::compileVarDec() {
	

	m_tokenizer.advance();
	
	if (m_tokenizer.keyWord() != "char" && m_tokenizer.keyWord() != "boolean" && m_tokenizer.keyWord() != "int" && m_tokenizer.keyWord() != "string" && m_tokenizer.identifier() == "None") 
		errorMesage("SyntaxError", "variable type", 2);

	std::string type;
	if (m_tokenizer.identifier() != "None") {
		
		type = m_tokenizer.identifier();
	}
	else {
		
		type = m_tokenizer.keyWord();
	}

	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);

	std::string name = m_tokenizer.identifier();
	sym_tab.defineSub(name, type, "local");

	m_tokenizer.advance();
	while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ";") {
		
		if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
		
		m_tokenizer.advance();
		if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
		name = m_tokenizer.identifier();
		sym_tab.defineSub(name, type, "local");

		m_tokenizer.advance();
	}
	
}

void CompilationEngine::compileStatements() {
	

	std::string token = m_tokenizer.keyWord();
	
	if (token != "let" && token != "if" && token != "while" && token != "do" && token != "return") {
		m_tokenizer.advance();
		if (m_tokenizer.symbol() == "}") {
			
			return;
		}
		errorMesage("SyntaxError", "statements", 2);
	}
	
	
	while (m_tokenizer.hasMoreTokens()) {
		if (token == "let") compileLet();
		else if (token == "if") compileIf();
		else if (token == "while") compileWhile();
		else if (token == "do") compileDo();
		else compileReturn();
		
		m_tokenizer.advance();
		
		token = m_tokenizer.keyWord();
		
		
		if (token != "let" && token != "if" && token != "while" && token != "do" && token != "return") break;
	}
	
}

void CompilationEngine::compileLet() {
	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
	std::string varName = m_tokenizer.identifier();
	
	if (sym_tab.kindOf(varName) == "None") errorMesage("SyntaxError", "", 3, "unknow identifier");

	m_tokenizer.advance();
	bool is_arr = false;
	if (m_tokenizer.symbol() == "[") {
		is_arr = true;
		writer.writePush(sym_tab.kindOf(varName), sym_tab.indexOf(varName));
		while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != "]") {
			
			compileExpression();
			m_tokenizer.advance();
		}
		writer.writeArithmetic("+");
		m_tokenizer.advance();
	}
	
	if (m_tokenizer.symbol() != "=") errorMesage("SyntaxError", "=", 2);

	compileExpression();
	if (is_arr) {
		writer.writePop("temp", 0);
		writer.writePop("pointer", 1);
		writer.writePush("temp", 0);
		writer.writePop("that", 0);
	}
	else
		writer.writePop(sym_tab.kindOf(varName), sym_tab.indexOf(varName));

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != ";") errorMesage("SyntaxError", ";", 2);
}

void CompilationEngine::compileIf() {

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "(") errorMesage("SyntaxError", "(", 2);
	
	compileExpression();
	int index = count_label + 2; //ind 2
	count_label += 2;
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != ")") errorMesage("SyntaxError", ")", 2);
	writer.writeArithmetic("not");
	writer.writeIf(class_name + "L" + std::to_string(index - 1)); //if-goto label 1

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);
	
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "}")
		compileStatements();

	if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "}", 2);
	writer.writeGoto(class_name + "L" + std::to_string(index)); //goto label 2
	writer.writeLabel(class_name + "L" + std::to_string(index - 1)); // label 1

	m_tokenizer.advance();
	if (m_tokenizer.keyWord() == "else") {
		m_tokenizer.advance();
		if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);
		m_tokenizer.advance();
		if (m_tokenizer.symbol() != "}")

			compileStatements();

		if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "}", 2);
		
	}
	else 
		m_tokenizer.back();

	writer.writeLabel(class_name + "L" + std::to_string(index)); // label 2
}

void CompilationEngine::compileWhile() {

	int index = count_label + 2;
	count_label += 2;
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "(") errorMesage("SyntaxError", "(", 2);
	writer.writeLabel(class_name + "L" + std::to_string(index - 1));
	compileExpression();

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != ")") errorMesage("SyntaxError", ")", 2);
	writer.writeArithmetic("not");
	writer.writeIf(class_name + "L" + std::to_string(index));

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "{") errorMesage("SyntaxError", "{", 2);

	m_tokenizer.advance();
	if (m_tokenizer.symbol() != "}")
		compileStatements();
	writer.writeGoto(class_name + "L" + std::to_string(index - 1));
	

	if (m_tokenizer.symbol() != "}") errorMesage("SyntaxError", "}", 2);
	writer.writeLabel(class_name + "L" + std::to_string(index));
	
}

void CompilationEngine::compileDo() {

	m_tokenizer.advance();
	if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
	std::string name = m_tokenizer.identifier();
	
	m_tokenizer.advance();
	if (m_tokenizer.symbol() == "(") {
		int nArgs = 1;
		writer.writePush("pointer", 0);

		m_tokenizer.advance();
		if (m_tokenizer.symbol() != ")") {
			m_tokenizer.back();
			compileExpression();
			m_tokenizer.advance();
			++nArgs;
		}

		while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ")") {
			if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
			compileExpression();
			m_tokenizer.advance();
			++nArgs;
		}
		writer.writeCall(class_name + "." + name, nArgs);
		writer.writePop("temp", 0);
	}
	
	else if (m_tokenizer.symbol() == ".") {

		m_tokenizer.advance();
		if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
		std::string method_name = m_tokenizer.identifier();
		int nArgs = 0;
		if (sym_tab.kindOf(name) != "None") {
			++nArgs;
			writer.writePush(sym_tab.kindOf(name), sym_tab.indexOf(name));
		}
		
		m_tokenizer.advance();
		if (m_tokenizer.symbol() == "(") {
			

			m_tokenizer.advance();
			if (m_tokenizer.symbol() != ")") {
				m_tokenizer.back();
				compileExpression();
				m_tokenizer.advance();
				++nArgs;
			}
			
			while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ")") {
				if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
				compileExpression();
				m_tokenizer.advance();
				++nArgs;
			}
			if (sym_tab.typeOf(name) != "None")
				writer.writeCall(sym_tab.typeOf(name) + "." + method_name, nArgs);
			else
				writer.writeCall((name) + "." + method_name, nArgs);

			writer.writePop("temp", 0);
		}
	}
	else errorMesage("SyntaxError", "statement", 2);
	
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != ";") errorMesage("SyntaxError", ";", 2);

}

void CompilationEngine::compileReturn() {
	m_tokenizer.advance();
	if (m_tokenizer.symbol() != ";") {
		m_tokenizer.back();
		compileExpression();
		m_tokenizer.advance();
	}
	else
		writer.writePush("constant", 0);

	if (m_tokenizer.symbol() != ";") errorMesage("SyntaxError", ";", 2);
	writer.writeReturn();
}

void CompilationEngine::compileExpression() {

	
	compileTerm();
	m_tokenizer.advance();
	
	while (m_tokenizer.symbol() == "+" || m_tokenizer.symbol() == "-" 
		|| m_tokenizer.symbol() == "*" || m_tokenizer.symbol() == "/" 
		|| m_tokenizer.symbol() == "&amp;" || m_tokenizer.symbol() == "|"  
		|| m_tokenizer.symbol() == "&lt;" || m_tokenizer.symbol() == "&gt;" 
		|| m_tokenizer.symbol() == "=") {
		std::string command = m_tokenizer.symbol();
		
		compileTerm();
		writer.writeArithmetic(command);

		m_tokenizer.advance();
	}

	m_tokenizer.back();
}

void CompilationEngine::compileTerm() {
	m_tokenizer.advance();

	if (m_tokenizer.intVal() != "None")
		writer.writePush("constant", atoi(m_tokenizer.intVal().c_str()));


	else if (m_tokenizer.stringVal() != "None") {
		std::string str = m_tokenizer.stringVal();
		writer.writePush("constant", str.length());
		writer.writeCall("String.new", 1);
		for (int i = 0; i < m_tokenizer.stringVal().length(); ++i) {
			writer.writePush("constant", str[i]);
			writer.writeCall("String.appendChar", 2);
		}
	}

	else if (m_tokenizer.keyWord() == "true" || m_tokenizer.keyWord() == "false"
		|| m_tokenizer.keyWord() == "this" || m_tokenizer.keyWord() == "null") {
		if (m_tokenizer.keyWord() == "true") {
			writer.writePush("constant", 0);
			writer.writeArithmetic("not");
		}

		else if ( m_tokenizer.keyWord() == "false" || m_tokenizer.keyWord() == "null")
			writer.writePush("constant", 0);

		else
			writer.writePush("pointer", 0);
	}
		
	

	else if (m_tokenizer.identifier() != "None") {
		
		std::string name = m_tokenizer.identifier();
		
		m_tokenizer.advance();

		if (m_tokenizer.symbol() == "[") {
			writer.writePush(sym_tab.kindOf(name), sym_tab.indexOf(name));

			compileExpression();
			writer.writeArithmetic("+");
			writer.writePop("pointer", 1);
			writer.writePush("that", 0);
			
			m_tokenizer.advance();
			
			if (m_tokenizer.symbol() != "]") errorMesage("SyntaxError", "]", 2);
		}

		else if (m_tokenizer.symbol() == "(") {
			int nArgs = 1;
			writer.writePush("pointer", 0);
		
			
			m_tokenizer.advance();
			while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ")") {
				compileExpression();
				m_tokenizer.advance();
				if (m_tokenizer.symbol() != ")") break;
				++nArgs;

				if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
				
				compileExpression();
				m_tokenizer.advance();
			}
			writer.writeCall(class_name + "." + name, nArgs);
			if (foo_table[name].data_type == "void") writer.writePop("temp", 0);
		}

		else if (m_tokenizer.symbol() == ".") {

			m_tokenizer.advance();
			if (m_tokenizer.identifier() == "None") errorMesage("SyntaxError", "identifier", 2);
			std::string method_name = m_tokenizer.identifier();

			m_tokenizer.advance();
			if (m_tokenizer.symbol() == "(") {
				int nArgs = 0;
				if (sym_tab.kindOf(name) != "None") {
					++nArgs;
					writer.writePush(sym_tab.kindOf(name), sym_tab.indexOf(name));
				}

				m_tokenizer.advance();
				if (m_tokenizer.symbol() != ")") {
					m_tokenizer.back();
					compileExpression();
					++nArgs;
					m_tokenizer.advance();
				}

				while (m_tokenizer.hasMoreTokens() && m_tokenizer.symbol() != ")") {
					if (m_tokenizer.symbol() != ",") errorMesage("SyntaxError", ",", 2);
					compileExpression();
					++nArgs;
					m_tokenizer.advance();
				}
				if (sym_tab.typeOf(name) != "None")
					writer.writeCall(sym_tab.typeOf(name) + "." + method_name, nArgs);
				else
					writer.writeCall((name) + "." + method_name, nArgs);

				if (foo_table[name].data_type == "void") writer.writePop("temp", 0);
			}
		}
		else {
			m_tokenizer.back();
			
			if (sym_tab.kindOf(name) == "None") errorMesage("SyntaxError", "", 3, "unknown identifier");
			writer.writePush(sym_tab.kindOf(name), sym_tab.indexOf(name));
		}
	}

	else if (m_tokenizer.symbol() == "(") {
		compileExpression();
		m_tokenizer.advance();
		if (m_tokenizer.symbol() != ")") errorMesage("SyntaxError", ")", 2);
	}

	else if (m_tokenizer.symbol() == "~" || m_tokenizer.symbol() == "-") {
		std::string command = m_tokenizer.symbol();
		compileTerm();
		if (command == "-") writer.writeArithmetic("neg");
		else writer.writeArithmetic("not");
	}

	else errorMesage("SyntaxError", "expression", 2);
}


void CompilationEngine::errorMesage(std::string type, std::string expected, int error_code, std::string met) {
	if (m_tokenizer.tokenType() == "identifier") met = m_tokenizer.identifier();
	else if (m_tokenizer.tokenType() == "keyword") met = m_tokenizer.keyWord();
	else if (m_tokenizer.tokenType() == "symbol") met = m_tokenizer.symbol();
	else if (m_tokenizer.tokenType() == "integerConstant") met = m_tokenizer.intVal();
	else if (m_tokenizer.tokenType() == "stringConstant") met = m_tokenizer.stringVal();
	std::cerr << m_filename + ".jack: " << type << " expected '"  << expected << "', met " << met << std::endl;
	exit(error_code);
}


