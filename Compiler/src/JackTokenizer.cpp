#include "../include/JackTokenizer.h"
#include "../include/Dictionary.h"


Tokenizer::Tokenizer(std::string filename) {
    input.open(filename + ".jack");
    if (!input) {
        input.close();
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
    fileName = filename;
    output.open(filename + "T.xml");
    if (!output) {
        std::cerr << "Error: Could not open file " << filename + "T.xml" << std::endl;
        exit(1);
    }
    std::map<std::string, std::string> dictionary = initialization();
    std::string code;
    bool isMultiComment = false;
    output << "<tokens>" << std::endl;
    while (std::getline(input, code)) {

        if (code.find("*/") != std::string::npos) {
            size_t endPos = code.find("*/");
            code.erase(0, endPos + 2);
            isMultiComment = false;
        }
        if (isMultiComment) continue;

        if (code.find("/*") != std::string::npos) {
            size_t startPos = code.find("/*");
            code.erase(startPos);
            isMultiComment = true;
        }

        std::regex regex0("//.*|/\\*.*?\\*/");
        std::string str = std::regex_replace(code, regex0, "");

        std::regex regex("\"([^\"]*)\"|\\s|\\b|\\W");
        std::sregex_token_iterator iter(str.begin(), str.end(), regex, { -1, 0 });

        std::vector<std::string> tokens{ iter, {} };
        tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
            [](const std::string& token) { return token.empty() || isWhitespace(token); }
        ), tokens.end());

        
        std::string codeT = "";
        for (int i = 0; i < tokens.size(); ++i) {
            
            if (dictionary.count(tokens[i]) == 1) {
                std::string type = dictionary[tokens[i]];
                codeT = "<" + type + "> " + tokens[i] + " </" + type + ">";
            }
            else if (tokens[i] == "<" || tokens[i] == ">") {
                std::string op = tokens[i] == "<" ? "&lt" : "&gt";
                codeT = "<symbol> " + op + "; </symbol>";
            }
            else if (tokens[i] == "&")
                codeT = "<symbol> &amp; </symbol>";

            else if (isDigitsOnly(tokens[i]))
                codeT = "<integerConstant> " + tokens[i] + " </integerConstant>";

            else if (tokens[i][0] == '"')
                codeT = "<stringConstant> " + tokens[i].substr(1, tokens[i].length() - 2) + " </stringConstant>";

            else 
                codeT = "<identifier> " + tokens[i] + " </identifier>";
            
            output << "  " << codeT << std::endl;
        }

    }
    output << "</tokens>";
    output.close();
    input.close();
}

bool Tokenizer::hasMoreTokens(){
    std::ifstream file(fileName + "T.xml");
    std::string line;

    for (int i = 1; i < countToken; ++i)
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!std::getline(file, line) || line == "</token>")
        return false;
    return true;
}

void Tokenizer::advance() {
    std::ifstream file(fileName + "T.xml");
    std::string line;
    ++countToken;
    for (int i = 1; i < countToken; ++i) 
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::getline(file, line);
    currentToken = line;
}

void Tokenizer::back() {
    std::ifstream file(fileName + "T.xml");
    std::string line;
    --countToken;
    for (int i = 1; i < countToken; ++i)
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(file, line);
    currentToken = line;
}

std::string Tokenizer::tokenType() {
    std::size_t start = currentToken.find('<');
    std::size_t end = currentToken.find('>', start + 1);
    return currentToken.substr(start + 1, end - start - 1);
}

std::string Tokenizer::keyWord() {
    if (currentToken.find("keyword") != std::string::npos) {
        std::size_t start = currentToken.find('>');
        std::size_t end = currentToken.find('<', start + 1);
        return currentToken.substr(start + 2, end - start - 3);
    }
    return "None";
}

std::string Tokenizer::symbol() {
    if (currentToken.find("symbol") != std::string::npos) {
        std::size_t start = currentToken.find('>');
        std::size_t end = currentToken.find('<', start + 1);
        return currentToken.substr(start + 2, end - start - 3);
    }
    return "None";
}

std::string Tokenizer::identifier() {
    if (currentToken.find("identifier") != std::string::npos) {
        std::size_t start = currentToken.find('>');
        std::size_t end = currentToken.find('<', start + 1);
        return currentToken.substr(start + 2, end - start - 3);
    }
    return "None";
}

std::string Tokenizer::intVal() {
    if (currentToken.find("integerConstant") != std::string::npos) {
        std::size_t start = currentToken.find('>');
        std::size_t end = currentToken.find('<', start + 1);
        return currentToken.substr(start + 2, end - start - 3);
    }
    return "None";
}

std::string Tokenizer::stringVal() {
    if (currentToken.find("stringConstant") != std::string::npos) {
        std::size_t start = currentToken.find('>');
        std::size_t end = currentToken.find('<', start + 1);
        return currentToken.substr(start + 2, end - start - 3);
    }
    return "None";
}

Tokenizer::Tokenizer(const Tokenizer& other)
    : fileName(other.fileName),
    currentToken(other.currentToken), countToken(other.countToken) {
    
}

Tokenizer& Tokenizer::operator=(const Tokenizer& other) {
    if (this != &other) {
        fileName = other.fileName;
        currentToken = other.currentToken;
        countToken = other.countToken;
    }
    return *this;
}