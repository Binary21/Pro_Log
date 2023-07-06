//=== Lexer.hpp - Lexer class definition ==============================//
//
// \file
// This file contains the definition of the Lexer class
//
//
//=====================================================================//

#ifndef LEXER_HPP
#define LEXER_HPP


#include <cstddef>
#include <list>
#include <ostream>
#include <string>
#include <map>
#include "Token.hpp"

using namespace vtpl;

class Lexer {
public:
	TokenList tokenize(std::istream&);

private:
	void DictionarySetup(std::map<char, TokenType>&);
	void StringUpdate(std::string& temp, int& lineValue, TokenList& tokenList);
};



#endif