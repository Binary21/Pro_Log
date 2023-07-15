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
		REQUIRE(tree.second.toString() == "(a(b,c,d))");
		REQUIRE(tree.first.isSet() == true);
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

	// Failed
	SECTION("unary predicate - variable")
	{
		string input = "f(a)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	/**
	// Failed
	SECTION("unary predicate - variable")
	{
		string input = "(f(X))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(X))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("binary predicate - fact")
	{
		string input = "(f(a,b))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a,b))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("binary predicate - variable")
	{
		string input = "(f(X,Y))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(X,Y))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("arbitrary tree 1")
	{
		string input = "(f(a,b(g,h)))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a,b(g,h)))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("arbitrary tree 2")
	{
		string input = "(f(a(b,c),d(e,f(g))))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a(b,c),d(e,f(g))))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("parse list of expressions")
	{
		string input = "(f(a),g(b),h(c))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a),g(b),h(c))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("parse complicated expression")
	{
		string input = "(f(a(b(c,d(e,f(g,h)))),i(j,k(l,m))))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a(b(c,d(e,f(g,h)))),i(j,k(l,m))))");
		REQUIRE(tree.first.isSet() == false);
		tree.first.message();
	}
	// Failed
	SECTION("parser error case - mismatched parens right")
	{
		string input = "(f(a,b)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	// Failed
	SECTION("parser error case - truncated input at paren")
	{
		string input = "(f(a,)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f(a))");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}
	// Failed
	SECTION("parser error case - invalid argument")
	{
		string input = "(f(123))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == true);
		tree.first.message();
	}**/
}
