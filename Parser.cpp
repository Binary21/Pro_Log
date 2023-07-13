#include "Parser.hpp"

#include <string>
#include <iostream>

using namespace std;
using namespace vtpl;

pair<ParseError, ExpressionTreeNode> vtpl::parseExpression(const TokenList& tokens)
{
	ParseError error;
	ExpressionTreeNode root;

	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();

	root = parseList(current, end, error);
	pair<ParseError, ExpressionTreeNode> temp = {error, root};
	return temp;
}

ExpressionTreeNode vtpl::parseList(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error) {
	string value = current->value();
	if (current != end) {
		current++;
	}
	if (current == end || current->type() != TokenType::OPEN) {
		return makeAtom(value);
	}
	else {
		if (current != end) {
			current++;
		}
		if (current == end || current->type() == TokenType::CLOSE)
		{
			error.set("Missmatched Parenthesis", *current);
			return makeAtom(value);
		}
		return makeCompound(value, parseChildren(current, end, error));
	}
}

list<ExpressionTreeNode> vtpl::parseChildren(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error) {
	list<ExpressionTreeNode> children;
	while (current != end && current->type() != TokenType::CLOSE) {
		children.push_back(parseList(current, end, error));
		if (current != end && current->type() == TokenType::COMMA) {
			current++;
			if (current == end || current->type() != TokenType::STRING)
				error.set("Invalid Argument After Comma", *current);
		}
		
	}
	if (current != end) {
		current++;
	} 
	else
	{
		error.set("Missmatched Parenthesis", *current);
	}
	return children;
}

// sets the error bool to true when there is an error
void ParseError::set(const char* str)
{
	errorMessage = str;
	error = true;
}

// uses the input token to write a token specific error message
void ParseError::set(const char* str, const Token token)
{
	errorMessage = string(str) + " on line " + to_string(token.line()) + " at: " + token.value();
	error = true;
}
// returns if there was an error
bool ParseError::isSet() const noexcept
{
	cout << "Error type: " << error << endl;
	return error;
}
// tells us the actual error message (error message is set in "void ParseError::set(const char* str = nullptr)"
string ParseError::message()
{
	cout << errorMessage << endl;
	return errorMessage;
}

pair<ParseError, ExpressionTreeNode> vtpl::parseExpression(const string& input)
{
	// use tokenizer to create a tokenlist then call all my helper functions
	pair<ParseError, ExpressionTreeNode> temp;
	return temp;
}
