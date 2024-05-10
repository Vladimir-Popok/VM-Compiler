#include "../include/CodeWriter.h"


CodeWriter::CodeWriter(Parser pars, string fileName) {
	m_fileName = fileName;
	if (pars.operation() == Parser::Type::ARITHMETIC_LOGIC) {
		string inst = pars.getOperation();
		if (inst == "add") m_code = template1(inst, "+");
		else if (inst == "sub") m_code = template1(inst, "-");
		else if (inst == "neg") m_code = template3(inst, "-");
		else if (inst == "eq") m_code = template2(inst, "JEQ");
		else if (inst == "gt") m_code = template2(inst, "JLT");
		else if (inst == "lt") m_code = template2(inst, "JGT");
		else if (inst == "and") m_code = template1(inst, "&");
		else if (inst == "or") m_code = template1(inst, "|");
		else if (inst == "not") m_code = template3(inst, "!");
	}

	else if (pars.operation() == Parser::Type::LABEL) 
		m_code = label(pars.getInst());
	

	else if (pars.operation() == Parser::Type::IF_GOTO) 
		m_code = if_goto(pars.getInst());
	

	else if (pars.operation() == Parser::Type::GOTO) 
		m_code = go_to(pars.getInst());
	

	else if (pars.operation() == Parser::Type::FUNCTION) 
		m_code = function(pars.getInst(), pars.getVariables());
	

	else if (pars.operation() == Parser::Type::CALL) 
		m_code = call(pars.getInst(), pars.getVariables());
	

	else if (pars.operation() == Parser::Type::RETURN) 
		m_code = returnFoo();
	 

	else {
		if ("pop" == pars.getInst()) m_code = pop(pars);
		if ("pop" == pars.getInst()) m_code = pop(pars);
		else m_code = push(pars);
	}
}

string CodeWriter::pop(Parser pars) {

	string segment, code = "", comment = "// pop " + pars.getSegment() + " " + pars.getAddress();

	if (pars.getSegment() == "pointer") {
		segment = (pars.getAddress() == "0" ? "THIS" : "THAT");

		code.append("@SP\n");
		code.append("M=M-1\n");
		code.append("A=M\n");
		code.append("D=M\n");
		code.append("@" + segment + "\n");
		code.append("M=D");
	}
	else if (pars.getSegment() == "static") {
		segment = m_fileName + "." + pars.getAddress();

		code.append("@SP\n");
		code.append("M=M-1\n");
		code.append("A=M\n");
		code.append("D=M\n");
		code.append("@" + segment + "\n");
		code.append("M=D");
	}

	else {
		segment = getCodeSegment(pars.getSegment());
		string aORm = (segment == "5" ? "A" : "M");

		code.append("@" + segment + "\n");
		code.append("D=" + aORm + "\n");
		code.append("@" + pars.getAddress() + "\n");
		code.append("D=D+A\n");
		code.append("@R13\n");
		code.append("M=D\n");
		code.append("@SP\n");
		code.append("M=M-1\n");
		code.append("A=M\n");
		code.append("D=M\n");
		code.append("@R13\n");
		code.append("A=M\n");
		code.append("M=D\n");

	}
	return comment + "\n" + code;
}

string CodeWriter::push(Parser pars) {

	string segment, code, comment = "// push " + pars.getSegment() + " " + pars.getAddress();

	if (pars.getSegment() == "pointer") {
		segment = (pars.getAddress() == "0" ? "THIS" : "THAT");

		code.append("@" + segment + "\n");
		code.append("D=M\n");
		code.append("@SP\n");
		code.append("A=M\n");
		code.append("M=D\n");
		code.append("@SP\n");
		code.append("M=M+1\n");
	}
	else if (pars.getSegment() == "static") {
		segment = m_fileName + "." + pars.getAddress();

		code.append("@" + segment + "\n");
		code.append("D=M\n"); // !!!
		code.append("@SP\n");
		code.append("A=M\n");
		code.append("M=D\n");
		code.append("@SP\n");
		code.append("M=M+1\n");
	}

	else if (pars.getSegment() == "constant") {

		code.append("@" + pars.getAddress() + "\n");
		code.append("D=A\n");
		code.append("@SP\n");
		code.append("A=M\n");
		code.append("M=D\n");
		code.append("@SP\n");
		code.append("M=M+1\n");
	}


	else {
		segment = getCodeSegment(pars.getSegment());
		string aORm = (segment == "5" ? "A" : "M");

		code.append("@" + pars.getAddress() + "\n");
		code.append("D=A\n"); // !!!
		code.append("@" + segment + "\n");
		code.append("A=D+" + aORm + "\n");
		code.append("D=M\n");
		code.append("@SP\n");
		code.append("A=M\n");
		code.append("M=D\n");
		code.append("@SP\n");
		code.append("M=M+1\n");
	}
	return comment + "\n" + code;
}

string CodeWriter::getCodeSegment(string segment) {
	if (segment == "local") return "LCL";
	if (segment == "argument") return "ARG";
	if (segment == "this") return "THIS";
	if (segment == "that") return "THAT";
	if (segment == "temp") return "5";
}

string CodeWriter::template1(string name, string type) {

	string code = "//" + name + "\n";

	code.append("@SP\n");
	code.append("AM=M-1\n");
	code.append("D=M\n");
	code.append("@R13\n");
	code.append("M=D\n");
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("D=M\n");
	code.append("@R13\n");
	code.append("D=D" + type + "M\n");
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("M=D\n");

	return code;
}

string CodeWriter::template2(string name, string type) {

	string code = "//" + name + "\n";
	code.append("@SP\n");
	code.append("AM=M-1\n");
	code.append("D=M\n");
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("D=D-M\n");
	code.append("@TRUE" + to_string(num_label) + "\n");
	code.append("D;" + type + "\n");
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("M=0\n");
	code.append("@END" + to_string(num_label) + "\n");
	code.append("0;JMP\n");
	code.append("(TRUE" + to_string(num_label) + ")\n");
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("M=-1\n");
	code.append("(END" + to_string(num_label++) + ")\n");

	return code;
}

string CodeWriter::template3(string name, string type) {
	string code = "//" + name + "\n";
	code.append("@SP\n");
	code.append("A=M-1\n");
	code.append("M=" + type + "M\n");

	return code;
}

string CodeWriter::label(string name) {
	string code = "// label\n";
	code.append("(" + name + ")\n");
	return code;
}

string CodeWriter::go_to(string name) {
	string code = "// goto\n";
	code.append("@" + name + "\n");
	code.append("0;JMP");
	return code;
}

string CodeWriter::if_goto(string name) {
	string code = "// if-goto " + name + "\n";
	code.append("@SP\n");
	code.append("AM=M-1\n");
	code.append("D=M\n");
	code.append("@" + name + "\n");
	code.append("D;JNE");
	return code;
}

string CodeWriter::function(string name, string var) {
	string code = "// function" + name + " " + var + "\n";
	code.append("(" + name + ")\n");
	for (int i = 0; i < atoi(var.c_str()); ++i) {
		code.append("@SP\n");
		code.append("A=M\n");
		code.append("M=0\n");
		code.append("@SP\n");
		code.append("M=M+1\n");
	}
	return code;
}

string CodeWriter::call(string name, string var) {
	
	string code = "// call " + name + " " + var + "\n";
	code.append("@" + name + "&ret." + to_string(foo_label) + "\n");
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
	code.append("@" + var + "\n");
	code.append("D=D-A\n");
	code.append("@ARG\n");
	code.append("M=D\n");

	code.append("@SP\n");
	code.append("D=M\n");
	code.append("@LCL\n");
	code.append("M=D\n");

	code.append("@" + name + "\n");
	code.append("0;JMP\n");
	code.append("(" + name + "&ret." + to_string(foo_label++) + ")\n");

	return code;
}

string CodeWriter::returnFoo() {

	string code = "// return\n";
	code.append("@LCL\n");
	code.append("D=M\n");
	code.append("@R13\n");
	code.append("M=D\n");

	code.append("@5\n");
	code.append("D=A\n");
	code.append("@R13\n");
	code.append("A=M-D\n");
	code.append("D=M\n");
	code.append("@R14\n");
	code.append("M=D\n");

	code.append("@SP\n");
	code.append("AM=M-1\n");
	code.append("D=M\n");
	code.append("@ARG\n");
	code.append("A=M\n");
	code.append("M=D\n")
		;
	code.append("@ARG\n");
	code.append("D=M+1\n");
	code.append("@SP\n");
	code.append("M=D\n");

	code.append("@1\n");
	code.append("D=A\n");
	code.append("@R13\n");
	code.append("A=M-D\n");
	code.append("D=M\n");
	code.append("@THAT\n");
	code.append("M=D\n");

	code.append("@2\n");
	code.append("D=A\n");
	code.append("@R13\n");
	code.append("A=M-D\n");
	code.append("D=M\n");
	code.append("@THIS\n");
	code.append("M=D\n");

	code.append("@3\n");
	code.append("D=A\n");
	code.append("@R13\n");
	code.append("A=M-D\n");
	code.append("D=M\n");
	code.append("@ARG\n");
	code.append("M=D\n");

	code.append("@4\n");
	code.append("D=A\n");
	code.append("@R13\n");
	code.append("A=M-D\n");
	code.append("D=M\n");
	code.append("@LCL\n");
	code.append("M=D\n");

	code.append("@R14\n");
	code.append("A=M\n");
	code.append("0;JMP\n");

	return code;
}