//=== Token.cpp - Token class implementation =================================//
//
/// \file
/// This file contains the implementation of the Token class and related
/// functions.
///
//============================================================================//
#include "Token.hpp"

#include <stdexcept>

using namespace vtpl;
  
Token::Token(TokenType type, std::size_t line) : m_type(type), m_line(line) {}

Token::Token(TokenType type, std::size_t line, const std::string &value)
    : m_type(type), m_line(line), m_value(value) {}

TokenType Token::type() const { return m_type; }

std::size_t Token::line() const { return m_line; }

const std::string & Token::value() const { return m_value; }
  
bool vtpl::operator==(const Token &t1, const Token &t2) {
  return (t1.type() == t2.type()) && (t1.line() == t2.line()) &&
         (t1.value() == t2.value());
}

bool vtpl::operator!=(const Token &t1, const Token &t2) { return !(t1 == t2); }

std::ostream & vtpl::operator<<(std::ostream &oss, const Token &t) {

  switch(t.type()){
  case TokenType::STRING:
    oss << "Token: type(STRING) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::OPEN:
    oss << "Token: type(OPEN) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::CLOSE:
    oss << "Token: type(CLOSE) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::COMMA:
    oss << "Token: type(COMMA) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::IMP:
    oss << "Token: type(IMP) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::END:
    oss << "Token: type(END) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  case TokenType::ERROR:
    oss << "Token: type(ERROR) value (" << t.value()
	<< ") from source line (" << t.line() << ")";
    break;
  default:
    throw std::logic_error("Invalid TokenType in operator<<");
  }
  
  return oss;
}
