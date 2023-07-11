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

namespace vtpl {

	TokenList tokenize(std::istream&);

	void dictionarySetup(std::map<char, TokenType>&);
	void stringUpdate(std::string& temp, int& lineValue, TokenList& tokenList);

class Lexer {
public:
	

private:
	
};

};


#endif