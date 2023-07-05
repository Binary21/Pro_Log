//=== Token.hpp - Token class definition =====================================//
//
/// \file
/// This file contains the definition of the Token class and TokenList type
/// definition.
///
/// A TokenList is produced by the Lexer module. See Lexer.hpp/cpp.
///
//============================================================================//
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>
#include <list>
#include <ostream>
#include <string>

namespace vtpl {
  
/* A token is one of 
STRING
OPEN ( 
CLOSE )
COMMA ,
IMP :-
END .
ERROR used to report lexing error
*/
enum class TokenType {
  STRING,
  OPEN,
  CLOSE,
  COMMA,
  IMP,
  END,
  ERROR
};

//============================================================================//
/// \brief A token has a type, value, and records the original source line on
/// which it appears.
///
//============================================================================//
class Token {
public:
  /// construct a token type on line with empty value
  Token(TokenType type, std::size_t line);

  /// construct a token type on line  with value
  Token(TokenType type, std::size_t line, const std::string &value);

  /// return the token type
  TokenType type() const;

  /// return the token's originating source line
  std::size_t line() const;

  /// return the token's value
  const std::string & value() const;

private:
  TokenType m_type;
  std::size_t m_line;
  std::string m_value;
};

// comparison operators for tokens, every field must match
bool operator==(const Token &t1, const Token &t2);
bool operator!=(const Token &t1, const Token &t2);

// convienience function for printing tokens
// this makes the Catch error reports more informative
std::ostream &operator<<(std::ostream &oss, const Token &t);

/// The token sequence is defined as a std::list of tokens.
typedef std::list<Token> TokenList;

} // end namespace vtpl

#endif
