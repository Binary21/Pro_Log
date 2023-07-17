#include "catch.hpp"

#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"

#include <sstream>
#include <string>

using namespace std;
using namespace vtpl;


TEST_CASE("Correct Tree formation")
{
	pair<ParseError, ExpressionTreeNode> tree;
	SECTION("Atom only")
	{
		string input = "(f)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "(a(b))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "(a(X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("binary predicate - variable")
	{
		string input = "(a(b,X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b,X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("arbitrary tree 1")
	{
		string input = "(a(b(c,d),c,e(f,g,h)))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c,d),c,e(f,g,h)))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("arbitrary tree 2")
	{
		string input = "(a(b(c(Y,e),f),X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("parse list of expressions")
	{
		string input = "(a(b(c(Y,e),f),X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("parse list of expressions")
	{
		string input = "(a(b,c,d,))";
		tree = parseExpression(input);
		REQUIRE(tree.first.isSet() == true);
	}
	
}

TEST_CASE("Error Testing")
{
	std::string input = "(f(a(b(c,d(e,f(g,h)))),i(j,k(l,m))))";
	pair<ParseError, ExpressionTreeNode> tree;

	for (std::size_t i = 0; i < input.size(); ++i)
	{
		std::string modifiedInput = input;
		modifiedInput[i] = ' ';
		tree = parseExpression(modifiedInput);
		cout << modifiedInput << endl;
		cout << tree.second.toString() << endl;
		REQUIRE(tree.first.isSet() == true);
	}

}
