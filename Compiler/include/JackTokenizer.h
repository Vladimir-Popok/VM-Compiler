#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <map>

class Tokenizer {

	std::ifstream input;
	std::ofstream output;
	std::string fileName;
	std::string currentToken = "";
	int countToken = 1;

public:
	Tokenizer(std::string filename);
	
	Tokenizer(const Tokenizer& other);  // Конструктор копирования
	Tokenizer& operator=(const Tokenizer& other);

	bool hasMoreTokens();
	void advance();
	void back();
	std::string tokenType();
	std::string keyWord();
	std::string symbol();
	std::string identifier();
	std::string intVal();
	std::string stringVal();
	int getLine() { return countToken; }
};