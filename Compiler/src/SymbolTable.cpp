#include "../include/SymbolTable.h"

void SymbolTable::reset() {
	subroutine_level.clear();
}

void SymbolTable::defineClass(std::string name, std::string type, std::string kind) {
	int index = varCountC(kind);
	Symbol data = { type, kind, index };
	class_level[name] = data;
}

void SymbolTable::defineSub(std::string name, std::string type, std::string kind) {
	int index = varCount(kind);
	Symbol data = { type, kind, index };
	subroutine_level[name] = data;
}

int SymbolTable::varCountC(std::string kind) {
	int count = 0;
	for (auto it : class_level)
		if (it.second.kind == kind) ++count;
	return count;
}


int SymbolTable::varCount(std::string kind) {
	int count = 0;
	for (auto it : subroutine_level) 
		if (it.second.kind == kind) ++count;
	return count;
}

std::string SymbolTable::kindOf(std::string name) {
	if (subroutine_level.count(name) == 1) return subroutine_level[name].kind;
	if (class_level.count(name) == 1) return class_level[name].kind;
	return "None";
}

std::string SymbolTable::typeOf(std::string name) {
	if (subroutine_level.count(name) == 1) return subroutine_level[name].type;
	if (class_level.count(name) == 1) return class_level[name].type;
	return "None";
}

int SymbolTable::indexOf(std::string name) {
	if (subroutine_level.count(name) == 1) return subroutine_level[name].index;
	if (class_level.count(name) == 1) return class_level[name].index;
	return -1;
}


void SymbolTable::show() {
	for (auto it : class_level)
		std::cout << it.first << "\t" << it.second.kind << "\t" << it.second.type << "\t" << it.second.index << std::endl;
	
}
