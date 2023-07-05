//=== LexerTests.cpp - Catch tests of tokenize implementation =================//
#include "catch.hpp"

#include <sstream>

#include "Token.hpp"

using namespace vtpl;

TEST_CASE("test token construction", "[Token]") {

  Token strToken = Token(TokenType::STRING, /*line =*/ 153, "name");

  REQUIRE(strToken.type() == TokenType::STRING);
  REQUIRE(strToken.line() == 153);
  REQUIRE(strToken.value() == "name");

  Token poToken = Token(TokenType::OPEN, /*line =*/ 153);

  REQUIRE(poToken.type() == TokenType::OPEN);
  REQUIRE(poToken.line() == 153);
  REQUIRE(poToken.value() == "");

  Token pcToken = Token(TokenType::CLOSE, /*line =*/ 153);

  REQUIRE(pcToken.type() == TokenType::CLOSE);
  REQUIRE(pcToken.line() == 153);
  REQUIRE(pcToken.value() == "");

  Token cToken = Token(TokenType::COMMA, /*line =*/ 153);

  REQUIRE(cToken.type() == TokenType::COMMA);
  REQUIRE(cToken.line() == 153);
  REQUIRE(cToken.value() == "");

  Token impToken = Token(TokenType::IMP, /*line =*/ 153);

  REQUIRE(impToken.type() == TokenType::IMP);
  REQUIRE(impToken.line() == 153);
  REQUIRE(impToken.value() == "");

  Token endToken = Token(TokenType::END, /*line =*/ 153);

  REQUIRE(endToken.type() == TokenType::END);
  REQUIRE(endToken.line() == 153);
  REQUIRE(endToken.value() == "");

  Token errToken = Token(TokenType::ERROR, /*line =*/ 42, "something bad happened");

  REQUIRE(errToken.type() == TokenType::ERROR);
  REQUIRE(errToken.line() == 42);
  REQUIRE(errToken.value() == "something bad happened");  
}

TEST_CASE("test comparison operators", "[Token]") {

  Token t1 = Token(TokenType::STRING, /*line =*/ 153, "name");
  Token t2 = Token(TokenType::STRING, /*line =*/ 351, "name");
  Token t3 = Token(TokenType::STRING, /*line =*/ 153, "anothername");
  Token t4 = Token(TokenType::STRING, /*line =*/ 153, "name");
  
  Token t5 = Token(TokenType::OPEN, /*line =*/ 153);

  Token t6 = Token(TokenType::CLOSE, /*line =*/ 153);

  Token t7 = Token(TokenType::COMMA, /*line =*/ 153);

  Token t8 = Token(TokenType::IMP, /*line =*/ 153);

  Token t9 = Token(TokenType::END, /*line =*/ 153);

  Token t10 = Token(TokenType::ERROR, /*line =*/ 42, "something bad happened");

  REQUIRE(t1 == t1);
  REQUIRE(t2 == t2);
  REQUIRE(t3 == t3);
  REQUIRE(t4 == t4);
  REQUIRE(t5 == t5);
  REQUIRE(t6 == t6);
  REQUIRE(t7 == t7);
  REQUIRE(t8 == t8);
  REQUIRE(t9 == t9);
  REQUIRE(t10 == t10);
  
  REQUIRE(t1 != t2);
  REQUIRE(t1 != t3);
  REQUIRE(t1 == t4);
  REQUIRE(t2 != t3);
  REQUIRE(t2 != t4);
  REQUIRE(t4 != t5);
  REQUIRE(t5 != t9);
  REQUIRE(t6 != t2);
  REQUIRE(t7 != t8);
  REQUIRE(t8 != t6);
  REQUIRE(t9 != t4);
}

TEST_CASE("test << operator", "[Token]") {

  Token t1 = Token(TokenType::STRING, /*line =*/ 153, "name");
  Token t2 = Token(TokenType::OPEN, /*line =*/ 153);
  Token t3 = Token(TokenType::CLOSE, /*line =*/ 153);
  Token t4 = Token(TokenType::COMMA, /*line =*/ 153);
  Token t5 = Token(TokenType::IMP, /*line =*/ 153);
  Token t6 = Token(TokenType::END, /*line =*/ 153);
  Token t7 = Token(TokenType::ERROR, /*line =*/ 42, "something bad happened");

  SECTION("render string token"){
    std::ostringstream oss;
    oss << t1;
    REQUIRE(oss.str() == "Token: type(STRING) value (name) from source line (153)");
  }

  SECTION("render open token"){
    std::ostringstream oss;
    oss << t2;
    REQUIRE(oss.str() == "Token: type(OPEN) value () from source line (153)");
  }

  SECTION("render close token"){
    std::ostringstream oss;
    oss << t3;
    REQUIRE(oss.str() == "Token: type(CLOSE) value () from source line (153)");
  }

  SECTION("render comma token"){
    std::ostringstream oss;
    oss << t4;
    REQUIRE(oss.str() == "Token: type(COMMA) value () from source line (153)");
  }

  SECTION("render implication token"){
    std::ostringstream oss;
    oss << t5;
    REQUIRE(oss.str() == "Token: type(IMP) value () from source line (153)");
  }

  SECTION("render end token"){
    std::ostringstream oss;
    oss << t6;
    REQUIRE(oss.str() == "Token: type(END) value () from source line (153)");
  }

  SECTION("render error token"){
    std::ostringstream oss;
    oss << t7;
    REQUIRE(oss.str() == "Token: type(ERROR) value (something bad happened) from source line (42)");
  }
}




