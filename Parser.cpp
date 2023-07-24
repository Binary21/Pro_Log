
#include "Parser.hpp"
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <tuple>
#include <algorithm>


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
	cout << printString(current) << endl;
	if (current->type() == TokenType::OPEN)
	{
		end--;
		if (end->type() == TokenType::CLOSE)
		{
			diffinput = true;
		}
		else
		{
			error.set("Missing close parenthesis");
		}
		current++;
	}
	ExpressionTreeNode rooter;
	cout << printString(current) << endl;
	rooter.type = ExpressionTreeNodeType::ROOT;
	root = parseList(current, end, error, depth, diffinput);
	if (current != end && current->type() != TokenType::COMMA)
	{
		error.set("error");
	}
	rooter.children = { root };
	if (current == end)
	{
		if (depth != 0)
			error.set("Incomplete input");
		rooter.children = { root };
		return { error, rooter };
	}

	while (current->type() == TokenType::COMMA && depth == 0)
	{
		if (current->type() == TokenType::COMMA)
		{
			current++;
			if (current == end)
			{
				error.set("Extra trailing comma");
			}
		}
		root = parseList(current, end, error, depth, diffinput);
		rooter.children.emplace_back(root);
	}
	if (current != end || depth != 0)
	{
		error.set("error trailing parenthesis");
	}
	
	return { error, rooter };
}

ExpressionTreeNode vtpl::parseList(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput) {
	string value = current->value();
	cout << value << endl;
	if (current->type() == TokenType::ERROR)
		error.set("Error, invalid argument type");
	if (current != end) {
		current++;
	}
	if (current == end)
		return makeAtom(value);
	if (current->type() == TokenType::STRING && current != end)
	{
		error.set("Missing Comma");
		if (isupper(value[0]))
		{
			return makeVariable(value);
		}
		cout << current->value() << endl;
		return makeAtom(value);
	}
	else if (current == end || current->type() != TokenType::OPEN) {
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
		if (current == end)
		{
			error.set("Truncated input");
			if (isupper(value[0]))
				{
					return makeVariable(value);
				}
			return makeAtom(value);
		}
		if (current->type() == TokenType::CLOSE)
		{
			depth--;
			error.set("Missmatched Parenthesis");
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		else if (current->type() == TokenType::COMMA)
		{
			error.set("incorrenct comma location");
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		else if (current->type() == TokenType::OPEN && depth > 0)
		{
			error.set("incorrenct comma location");
			if (isupper(value[0]))
			{
				return makeVariable(value);
			}
			return makeAtom(value);
		}
		list<ExpressionTreeNode> compound = parseChildren(current, end, error, depth, diffinput);
		for (ExpressionTreeNode node : compound)
		{
			cout << node.contents << endl;
		}
		if (compound.size() == 0)
			return makeAtom(value);
		return makeCompound(value, compound);
	}
}

list<ExpressionTreeNode> vtpl::parseChildren(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput) {
	list<ExpressionTreeNode> children;
	while (current != end && current->type() != TokenType::CLOSE) {
		ExpressionTreeNode value = parseList(current, end, error, depth, diffinput);
		children.push_back(value);
		if (current != end && current->type() == TokenType::STRING)
		{
			error.set("error");
		}
		else if (current != end && current->type() == TokenType::COMMA) {
			current++;
			if (current == end || current->type() != TokenType::STRING)
			{
				error.set("Invalid Argument After Comma");
			}
		}
	}

	if (current != end) {
		current++;
		depth--;
	}
	else if (diffinput)
	{
		error.set("Missmatched Parenthesis");
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
	return error;
}
// tells us the actual error message (error message is set in "void ParseError::set(const char* str = nullptr)"
string ParseError::message()
{
	return errorMessage;
}

pair<ParseError, ExpressionTreeNode> vtpl::parseExpression(const string& input)
{
	istringstream iss(input);
	return parseExpression(tokenize(iss));
}
pair<ParseError, ExpressionTreeNode> vtpl::parseQuery(const TokenList& tokens)
{
	cout << "Address of tokens in parseQuery: " << &tokens << endl;
	ParseError error;
	ExpressionTreeNode root;
	bool diffinput = false;
	int depth = 0;
	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();

	for (Token token : tokens)
	{
		if (token.type() == TokenType::END)
		{
			cout << "this pidgeon crazy" << endl;
		}
	}

	end--;
	ExpressionTreeNode rooter;
	rooter.type = ExpressionTreeNodeType::ROOT;
	if (end->type() == TokenType::END)
	{
		root = parseList(current, end, error, depth, diffinput);
		if (current != end && current->type() != TokenType::COMMA)
		{
			error.set("error");
		}
		rooter.children = { root };
		while (current->type() == TokenType::COMMA && depth == 0)
		{
			if (current->type() == TokenType::COMMA)
			{
				current++;
				if (current == end)
				{
					error.set("Extra trailing comma");
				}
			}
			root = parseList(current, end, error, depth, diffinput);
			rooter.children.emplace_back(root);
		}
		return { error, rooter };
	}
	else
	{
		error.set("Error with Query");
		return { error, root };
	}
}
pair<ParseError, ExpressionTreeNode> vtpl::parseQuery(const string& input)
{
	ParseError error;
	ExpressionTreeNode root;
	string sendIn = input.substr(0, input.find_last_not_of(' ') + 1);
	if (sendIn[sendIn.size() - 1] == '.')
	{
		sendIn.pop_back();
		istringstream iss(sendIn);

		return parseExpression(tokenize(iss));
	}
	else
	{
		error.set("Error with Query");
	}
	return { error, root };
	
}

pair<TokenList::const_iterator, TokenList::const_iterator> vtpl::Delimiter(TokenList::const_iterator current, TokenList::const_iterator it)
{
	auto delimiter = std::find_if(current, it,
		[](const Token& token) { return token.type() == TokenType::IMP; });

	// If IMP not found, return the end iterator
	if (delimiter == it) {
		return { it, it };
	}
	else {
		return { delimiter, next(delimiter) };
	}
}

std::tuple<ParseError, vtpl::KnowledgeBase> vtpl::parseKnowledgeBase(const TokenList& tokens)
{
	Clause clause;
	ParseError error;
	KnowledgeBase knowldgeBase;
	pair<ParseError, ExpressionTreeNode> head;
	pair<ParseError, ExpressionTreeNode> body;
	
	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();
	TokenList::const_iterator delimiter1 = tokens.end();
	TokenList::const_iterator delimiter2 = tokens.end();
	end--;
	for (TokenList::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		//cout << "current value: " << printString(current) << endl;
		//cout << "it value: " << printString(it) << endl;
		//cout << "delimiter1: " << printString(delimiter1) << endl;
		//cout << "delimiter2: " << printString(delimiter2) << endl;
		if (it->type() == TokenType::END)
		{
			//auto delimiter = Delimiter(current, it);

			TokenList headTokens(current, delimiter1);
			for (Token token : headTokens)
			{
				cout << printString(token);
			}
			cout << endl;
			head = parseExpression(headTokens);
			cout << head.second.toString() << endl;
			clause.head = head.second.children.front();
			if (delimiter2 != tokens.end())
			{
				TokenList bodyTokens(delimiter2, next(it));
				for (Token token : bodyTokens)
				{
					cout << printString(token);
				}
				cout << endl;
				body = parseQuery(bodyTokens);
				cout << body.second.toString() << endl;
				clause.body = body.second;
			}
			else if (it != end)
				it++;

			if (head.first.isSet() || body.first.isSet())
			{
				error.set("an error occurred and the KnowledgeBase may be incomplete");
			}

			knowldgeBase.tell(clause);
			
			if (it != end)
			{
				current = it;
				current++;
				//cout << "current the issue " << printString(current) << endl;
			}
				
			if(current->type() == TokenType::STRING)
				cout << "shaboogy" << endl;
		}
		if (it->type() == TokenType::IMP)
		{
			delimiter1 = it;
			if (delimiter1 != tokens.begin())
			{
				delimiter1;
			}
			delimiter2 = it;
			if (delimiter2 != tokens.end())
			{
				delimiter2++;
			}
		}
	}

	return { error, knowldgeBase };
}

std::tuple<ParseError, vtpl::KnowledgeBase> vtpl::parseKnowledgeBase(const string& input)
{
	istringstream iss(input);
	return parseKnowledgeBase(tokenize(iss));
}

string vtpl::printString(TokenList::const_iterator& current)
{
	if (current->type() == TokenType::CLOSE)
		return ")";
	if (current->type() == TokenType::OPEN)
		return "(";
	if (current->type() == TokenType::COMMA)
		return ",";
	if (current->type() == TokenType::STRING)
		return current->value();
	if (current->type() == TokenType::END)
		return ".";
	return " ";
}

string vtpl::printString(Token& token)
{
	if (token.type() == TokenType::CLOSE)
		return ")";
	if (token.type() == TokenType::OPEN)
		return "(";
	if (token.type() == TokenType::COMMA)
		return ",";
	if (token.type() == TokenType::STRING)
		return token.value();
	if (token.type() == TokenType::END)
		return ".";
	return " ";
}