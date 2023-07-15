#include "Parser.hpp"

#include <sstream>
#include <string>
#include <iostream>

using namespace std;
using namespace vtpl;

pair<ParseError, ExpressionTreeNode> vtpl::parseExpression(const TokenList& tokens)
{
	ParseError error;
	ExpressionTreeNode root;
	bool diffinput = false;
	int depth = 0;
	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();
	end--;

	if (current->type() == TokenType::OPEN)
	{
		if (end->type() == TokenType::CLOSE)
		{
			diffinput = true;
		}
		current++;
	}
	ExpressionTreeNode rooter;
	rooter.type = ExpressionTreeNodeType::ROOT;

	root = parseList(current, end, error, depth, diffinput);
	if (current != end && current->type() != TokenType::COMMA)
	{
		error.set("error", *current);
	}
	rooter.children = { root };
	while (current->type() == TokenType::COMMA && depth == 0)
	{
		current++;
		root = parseList(current, end, error, depth, diffinput);
		rooter.children.emplace_back(root);
	}
	return { error, rooter };
}

ExpressionTreeNode vtpl::parseList(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput) {
	string value = current->value();
	cout << value << endl;
	if (current != end) {
		current++;
	}
	if (current->type() == TokenType::CLOSE && depth <= 0 && current != end)
	{
		
		error.set("Missmatched Parenthesis", *current);
		depth--;
		if (isupper(value[0]))
		{
			return makeVariable(value);
		}
		return makeAtom(value);
	}
	if (current == end || current->type() != TokenType::OPEN) {
		if (isupper(value[0]))
		{
			return makeVariable(value);
		}
		return makeAtom(value);
	}
	if (current->type() == TokenType::CLOSE && depth <= 0)
	{
		error.set("Missmatched Parenthesis", *current);
		if (isupper(value[0]))
		{
			return makeVariable(value);
		}
		return makeAtom(value);
	}
	else {
		depth++;
		if (current != end) {
			current++;
		}
		if (current == end || current->type() == TokenType::CLOSE)
		{
			depth--;
			error.set("Missmatched Parenthesis", *current);
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		if (current->type() == TokenType::COMMA)
		{
			error.set("incorrenct comma location", *current);
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		if (current->type() == TokenType::OPEN && depth > 0)
		{
			error.set("incorrenct comma location", *current);
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		list<ExpressionTreeNode> compound = parseChildren(current, end, error, depth, diffinput);
		if (compound.size() == 0)
			return makeAtom(value);
		return makeCompound(value, compound);
	}
}

list<ExpressionTreeNode> vtpl::parseChildren(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput) {
	list<ExpressionTreeNode> children;
	while (current != end && current->type() != TokenType::CLOSE) {
		children.push_back(parseList(current, end, error, depth, diffinput));
		if (current != end && current->type() == TokenType::COMMA) {
			current++;
			if (current == end || current->type() != TokenType::STRING)
			{
				error.set("Invalid Argument After Comma", *current);
				return children;
			}
		}
	}

	if (current != end) {
		current++;
		depth--;
	}
	else if (diffinput)
	{
		error.set("Missmatched Parenthesis", *current);
		return {};
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
	istringstream iss(input);
	return parseExpression(tokenize(iss));
}