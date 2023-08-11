#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include "Lexer.hpp"
#include "ExpressionTree.hpp"
#include "KnowledgeBase.hpp"
#include "Token.hpp"
#include <tuple>

using namespace std;


class ParseError {
public:
	void set(const char* str = nullptr);
	void set(const char* str, const vtpl::Token token);

	bool isSet() const noexcept;

	string message();
private:
	bool error = false;
	string errorMessage = "";
};

namespace vtpl {
	pair<ParseError, ExpressionTreeNode> parseExpression(const TokenList& tokens);
	ExpressionTreeNode parseList(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput);
	list<ExpressionTreeNode> parseChildren(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput);

	void dictionarySetup(std::map<TokenType, char>&);

	pair<ParseError, ExpressionTreeNode> parseExpression(const string& input);
	std::pair<ParseError, ExpressionTreeNode> parseQuery(const TokenList& tokens);
	std::pair<ParseError, ExpressionTreeNode> parseQuery(const string& input);

	std::tuple<ParseError, vtpl::KnowledgeBase> parseKnowledgeBase(const TokenList& tokens);
	std::tuple<ParseError, vtpl::KnowledgeBase> parseKnowledgeBase(const string& input);

	pair< TokenList::const_iterator, TokenList::const_iterator> Delimiter(TokenList::const_iterator current, TokenList::const_iterator it);


	ExpressionTreeNode type(string value);
};

#endif