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
	}

	SECTION("unary predicate - fact")
	{
		string input = "(a(b))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b))");
	}

	SECTION("unary predicate - fact")
	{
		string input = "(a(X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(X))");
	}

	SECTION("binary predicate - variable")
	{
		string input = "(a(b,X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b,X))");
	}

	SECTION("arbitrary tree 1")
	{
		string input = "(a(b(c,d),c,e(f,g,h)))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c,d),c,e(f,g,h)))");
	}

	SECTION("arbitrary tree 2")
	{
		string input = "(a(b(c(Y,e),f),X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
	}

	SECTION("parse list of expressions")
	{
		string input = "(a(b(c(Y,e),f),X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
	}

	SECTION("parse list of expressions")
	{
		string input = "(a(b,c,d,))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b,c,d))");
	}
}

TEST_CASE("Error Tree formation")
{
	pair<ParseError, ExpressionTreeNode> tree;
	
	SECTION("Single atom")
	{
		string input = "(f)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	SECTION("Single atom - error parenthesis complete")
	{
		string input = "(f())";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Single atom - error parenthesis open")
	{
		string input = "(f()";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Single atom - error parenthesis close")
	{
		string input = "(f))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Single atom - error")
	{
		string input = "(f(,))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Compound One child")
	{
		string input = "(f(a))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	SECTION("Compound One child - error comma before")
	{
		string input = "(f(,a))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Compound One child - error comma after")
	{
		string input = "(f(a,))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a))");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Single atom - error parenthesis close")
	{
		string input = "(f(a)))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a))");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	SECTION("Single atom - error parenthesis close")
	{
		string input = "(f(((a))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
}
