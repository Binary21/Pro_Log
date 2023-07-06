//=== LexerTests.cpp - Catch tests of tokenize implementation
//=================//
#include "catch.hpp"

#include "Lexer.hpp"

#include <map>
#include <sstream>
#include <string>

using namespace vtpl;

Lexer lexer;

TEST_CASE("Test empty stream", "[Lexer]") {

  SECTION("empty stream") {
    std::string input = "";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 0);
  }

  SECTION("input stream with just whitespace and newlines") {
    std::string input = "    \t    \r      ";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 0);
  }
}

TEST_CASE("test one string", "[Lexer]") {

  std::string input = "hello";
  std::istringstream iss(input);

  TokenList tl = lexer.tokenize(iss);

  REQUIRE(tl.size() == 1);

  TokenList::iterator it = tl.begin();
  REQUIRE(*it == Token(TokenType::STRING, 1, "hello"));
}

TEST_CASE("test simple strings, one per line", "[Lexer]") {

  std::string input = "this\nis\na\nsequence\nof\nlines\n";
  std::istringstream iss(input);

  TokenList tl = lexer.tokenize(iss);

  REQUIRE(tl.size() == 6);
  REQUIRE(tl.back().type() != TokenType::ERROR);

  TokenList::iterator it = tl.begin();
  REQUIRE(*it == Token(TokenType::STRING, 1, "this"));
  REQUIRE(*(++it) == Token(TokenType::STRING, 2, "is"));
  REQUIRE(*(++it) == Token(TokenType::STRING, 3, "a"));
  REQUIRE(*(++it) == Token(TokenType::STRING, 4, "sequence"));
  REQUIRE(*(++it) == Token(TokenType::STRING, 5, "of"));
  REQUIRE(*(++it) == Token(TokenType::STRING, 6, "lines"));
}

TEST_CASE("Test comments", "[Lexer]") {

  std::string input = R"(
% this is a comment followed by a string
example% with an inline comment
  % and one more thing to say
)";

  std::istringstream iss(input);

  TokenList tl = lexer.tokenize(iss);

  REQUIRE(tl.size() == 1);
  REQUIRE(tl.back().type() != TokenType::ERROR);

  TokenList::iterator it = tl.begin();
  INFO(input);
  REQUIRE(*it == Token(TokenType::STRING, 3, "example"));
}

TEST_CASE("test fact lexing", "[Lexer]") {

  SECTION("atom only") {
    std::string input = "student.";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 2);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "student"));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
  }

  SECTION("unary predicate") {
    std::string input = "person(bob).";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 5);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "bob"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
  }

  SECTION("another unary predicate") {
    std::string input = "person ( sue ) . \n";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 5);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "sue"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
  }

  SECTION("unary predicate with whitespace interspersed") {
    std::string input = "person\n(\nsally\n)\n.\n";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 5);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 2));
    REQUIRE(*(++it) == Token(TokenType::STRING, 3, "sally"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 4));
    REQUIRE(*(++it) == Token(TokenType::END, 5));
  }

  SECTION("malformed implication") {
    std::string input = "person(bob:).";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 4);
    REQUIRE(tl.back().type() == TokenType::ERROR);
    REQUIRE(tl.back().line() == 1);
  }

  SECTION("two unary predicates") {
    std::string input = "person(bob).\nperson(sue)";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 9);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "bob"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 2));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "sue"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 2));
  }
}

TEST_CASE("Test rules", "[Lexer]") {

  SECTION("Testing single rule") {
    std::string input = "mortal(X) :- person(X).";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 10);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::IMP, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
  }

  SECTION("Testing multiple rules") {
    std::string input = R"(mortal(X) :- person(X).mortal(X) :- person(X).
mortal(X) :- person(X).
)";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 30);
    REQUIRE(tl.back().type() != TokenType::ERROR);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::IMP, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::IMP, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 2));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 2));
    REQUIRE(*(++it) == Token(TokenType::IMP, 2));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 2));
    REQUIRE(*(++it) == Token(TokenType::STRING, 2, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 2));
    REQUIRE(*(++it) == Token(TokenType::END, 2));
  }

  SECTION("Testing malformed rule - space") {
    std::string input = R"(mortal(X) :- person(X).mortal(X) : - person(X).
mortal(X) :- person(X).
)";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 15);
    REQUIRE(tl.back().type() == TokenType::ERROR);
    REQUIRE(tl.back().line() == 1);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::IMP, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
  }

  SECTION("Testing malformed rule - newline") {
    std::string input = R"(mortal(X) :- person(X).mortal(X) : 
- person(X).
mortal(X) :- person(X).
)";
    std::istringstream iss(input);

    TokenList tl = lexer.tokenize(iss);

    REQUIRE(tl.size() == 15);
    REQUIRE(tl.back().type() == TokenType::ERROR);
    REQUIRE(tl.back().line() == 1);

    TokenList::iterator it = tl.begin();
    REQUIRE(*it == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::IMP, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "person"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
    REQUIRE(*(++it) == Token(TokenType::END, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "mortal"));
    REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
    REQUIRE(*(++it) == Token(TokenType::STRING, 1, "X"));
    REQUIRE(*(++it) == Token(TokenType::CLOSE, 1));
  }
}

TEST_CASE("Test compound lexing", "[Lexer]") {

  std::string input = "a( b, c , ";
  std::istringstream iss(input);

  TokenList tl = lexer.tokenize(iss);

  REQUIRE(tl.size() == 6);
  REQUIRE(tl.back().type() != TokenType::ERROR);

  TokenList::iterator it = tl.begin();
  REQUIRE(*it == Token(TokenType::STRING, 1, "a"));
  REQUIRE(*(++it) == Token(TokenType::OPEN, 1));
  REQUIRE(*(++it) == Token(TokenType::STRING, 1, "b"));
  REQUIRE(*(++it) == Token(TokenType::COMMA, 1));
  REQUIRE(*(++it) == Token(TokenType::STRING, 1, "c"));
  REQUIRE(*(++it) == Token(TokenType::COMMA, 1));
}
