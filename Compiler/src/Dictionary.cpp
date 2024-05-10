#include "../include/Dictionary.h"

std::map<std::string, std::string> initialization() {
    std::map<std::string, std::string> g_dictionary;
    g_dictionary["class"] = "keyword";
    g_dictionary["constructor"] = "keyword";
    g_dictionary["function"] = "keyword";
    g_dictionary["method"] = "keyword";
    g_dictionary["field"] = "keyword";
    g_dictionary["static"] = "keyword";
    g_dictionary["var"] = "keyword";
    g_dictionary["int"] = "keyword";
    g_dictionary["char"] = "keyword";
    g_dictionary["boolean"] = "keyword";
    g_dictionary["void"] = "keyword";
    g_dictionary["true"] = "keyword";
    g_dictionary["false"] = "keyword";
    g_dictionary["null"] = "keyword";
    g_dictionary["this"] = "keyword";
    g_dictionary["let"] = "keyword";
    g_dictionary["do"] = "keyword";
    g_dictionary["if"] = "keyword";
    g_dictionary["else"] = "keyword";
    g_dictionary["while"] = "keyword";
    g_dictionary["return"] = "keyword";
    g_dictionary["{"] = "symbol";
    g_dictionary["}"] = "symbol";
    g_dictionary["("] = "symbol";
    g_dictionary[")"] = "symbol";
    g_dictionary["["] = "symbol";
    g_dictionary["]"] = "symbol";
    g_dictionary["."] = "symbol";
    g_dictionary[","] = "symbol";
    g_dictionary[";"] = "symbol";
    g_dictionary["+"] = "symbol";
    g_dictionary["-"] = "symbol";
    g_dictionary["="] = "symbol";
    g_dictionary["*"] = "symbol";
    g_dictionary["/"] = "symbol";
    g_dictionary["|"] = "symbol";
    g_dictionary["="] = "symbol";
    g_dictionary["~"] = "symbol";
    return g_dictionary;
}

bool isWhitespace(std::string s) {
	for (int index = 0; index < s.length(); index++) {
		if (!std::isspace(s[index]))
			return false;
	}
	return true;
}

bool isDigitsOnly(const std::string& str) {
	for (char c : str) {
		if (!std::isdigit(c)) {
			return false;
		}
	}
	return true;
}