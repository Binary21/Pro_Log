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
	if (current == end)
	{
		error.set("Parse Error: Invalid Head");
		return { error, root };
	}
	else if (current->type() == TokenType::OPEN)
	{
		end--;
		if (end->type() == TokenType::CLOSE)
		{
			diffinput = true;
		}
		else
		{
			error.set("Parse Error: Missing close parenthesis");
		}
		current++;
	}
	ExpressionTreeNode rooter;
	rooter.type = ExpressionTreeNodeType::ROOT;
	root = parseList(current, end, error, depth, diffinput);
	if (current != end && current->type() != TokenType::COMMA)
	{
		error.set("Parse Error: error 1");
	}
	rooter.children = { root };
	if (current == end)
	{
		if (depth != 0)
			error.set("Parse Error: Incomplete input");
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
				error.set("Parse Error: Extra trailing comma");
			}
		}
		root = parseList(current, end, error, depth, diffinput);
		rooter.children.emplace_back(root);
		if (current == end)
			break;
	}
	if (current != end || depth != 0)
	{
		error.set("Parse Error: trailing parenthesis");
	}

	return { error, rooter };
}

ExpressionTreeNode vtpl::parseList(TokenList::const_iterator& current, TokenList::const_iterator& end, ParseError& error, int& depth, bool diffinput) {
	string value = current->value();
	if (current->type() == TokenType::ERROR)
		error.set("Error, invalid argument type");
	if (current != end) {
		current++;
	}
	if (current == end)
		return makeAtom(value);
	if (current->type() == TokenType::STRING && current != end)
	{
		error.set("Parse Error: Missing Comma");
		return type(value);
	}
	else if (current == end || current->type() != TokenType::OPEN) {
		return type(value);
	}
	else {
		depth++;
		current++;

		if (current == end)
		{
			error.set("Parse Error: Truncated input");
			return type(value);
		}
		if (current->type() == TokenType::CLOSE)
		{
			depth--;
			error.set("Parse Error: Missmatched Parenthesis");
			return type(value);
		}
		else if (current->type() == TokenType::COMMA)
		{
			error.set("Parse Error: incorrenct comma location");
			return type(value);
		}
		else if (current->type() == TokenType::OPEN && depth > 0)
		{
			error.set("Parse Error: incorrenct comma location");
			return type(value);
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
		ExpressionTreeNode value = parseList(current, end, error, depth, diffinput);
		children.push_back(value);
		if (current != end && current->type() == TokenType::STRING)
		{
			error.set("Parse Error: 2");
		}
		else if (current != end && current->type() == TokenType::COMMA) {
			current++;
			if (current == end || current->type() != TokenType::STRING)
			{
				error.set("Parse Error: Invalid Argument After Comma");
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
		error.set("Parse Error: Missmatched Parenthesis");
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
	ParseError error;
	ExpressionTreeNode root;
	bool diffinput = false;
	int depth = 0;
	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();

	end--;
	ExpressionTreeNode rooter;
	rooter.type = ExpressionTreeNodeType::ROOT;
	if (end->type() == TokenType::END)
	{
		root = parseList(current, end, error, depth, diffinput);
		if (current != end && current->type() != TokenType::COMMA)
		{
			cout << printString(current) << endl;
			error.set("Parse Error: ");
		}
		rooter.children = { root };
		while (current->type() == TokenType::COMMA && depth == 0)
		{
			if (current->type() == TokenType::COMMA)
			{
				current++;
				if (current == end)
				{
					error.set("Parse Error: Extra trailing comma");
				}
			}
			root = parseList(current, end, error, depth, diffinput);
			rooter.children.emplace_back(root);
		}
		return { error, rooter };
	}
	else
	{
		error.set("Parse Error: with Query");
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
		error.set("Parse Error: with Query");
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
	auto it = tokens.begin();
	while (it != tokens.end())
	{
		//cout << printString(it) << endl;
		it++;
	}
	ParseError error;
	KnowledgeBase knowldgeBase;
	pair<ParseError, ExpressionTreeNode> head;
	pair<ParseError, ExpressionTreeNode> body;

	TokenList::const_iterator current = tokens.begin();
	TokenList::const_iterator end = tokens.end();
	TokenList::const_iterator delimiter1 = tokens.begin();
	TokenList::const_iterator delimiter2 = tokens.end();
	bool delimiterHit = false;
	if (end != current)
		end--;
	for (TokenList::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if (it->type() == TokenType::END)
		{
			Clause clause;
			//auto delimiter = Delimiter(current, it);
			TokenList headTokens;

			if (delimiterHit)
			{
				headTokens = TokenList(current, delimiter1);
			}
			else
			{
				headTokens = TokenList(current, it);
			}
			head = parseExpression(headTokens);
			if (head.first.isSet() || head.second.children.size() > 1)
			{
				error.set("Error: an error occurred and the KnowledgeBase may be incomplete");
				cout << "head: " << head.first.message() << endl;
				return { error, knowldgeBase };
			}
			clause.head = head.second.children.front();
			if (delimiter2 != tokens.end() && delimiterHit)
			{
				TokenList bodyTokens(delimiter2, next(it));
				body = parseQuery(bodyTokens);
				clause.body = body.second;
			}

			if (body.first.isSet())
			{
				error.set("Error: an error occurred and the KnowledgeBase may be incomplete");
				cout << "body: " << body.first.message() << endl;
				return { error, knowldgeBase };
			}

			knowldgeBase.tell(clause);

			if (it != end)
			{
				current = it;
				current++;
				delimiterHit = false;
			}
		}
		if (it->type() == TokenType::IMP)
		{
			delimiter1 = it;
			delimiter2 = it;
			if (delimiter2 != tokens.end())
			{
				delimiter2++;
			}
			delimiterHit = true;
		}
	}
	if (knowldgeBase.size() == 0)
		error.set("Error: an error occurred and the KnowledgeBase may be incomplete");
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

ExpressionTreeNode vtpl::type(string value)
{
	if (isupper(value[0]))
	{
		return makeVariable(value);
	}
	return makeAtom(value);
}