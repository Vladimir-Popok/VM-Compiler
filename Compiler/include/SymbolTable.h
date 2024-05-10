#pragma once
#include <iostream>
#include <unordered_map>

struct Symbol {
	std::string type;
	std::string kind;
	int index;
};

class SymbolTable {
	std::unordered_map <std::string, Symbol> class_level;
	std::unordered_map <std::string, Symbol> subroutine_level;

public:
	
	void reset();

	void defineClass(std::string name, std::string type, std::string kind);

	void defineSub(std::string name, std::string type, std::string kind);

	int varCountC(std::string kind);

	int varCount(std::string kind);

	std::string kindOf(std::string name);

	std::string typeOf(std::string name);

	int indexOf(std::string name);

	void show();
};