#include "catch.hpp"

#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"
#include "KnowledgeBase.hpp"
#include <sstream>
#include <string>

using namespace std;
using namespace vtpl;

TEST_CASE("Correct Tree formation")
{
	
	pair<ParseError, ExpressionTreeNode> tree;
	SECTION("Atom only")
	{
		string input = "f";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(f)");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "a(b)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "a(X)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("binary predicate - variable")
	{
		string input = "a(b,X)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b,X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("arbitrary tree 1")
	{
		string input = "a(b(c,d),c,e(f,g,h))";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c,d),c,e(f,g,h)))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("arbitrary tree 2")
	{
		string input = "a(b(c(Y,e),f),X)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("parse list of expressions")
	{
		//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		string input = "a(b(c(Y,e),f),X)";
		tree = parseExpression(input);
		REQUIRE(tree.second.toString() == "(a(b(c(Y,e),f),X))");
		REQUIRE(tree.first.isSet() == false);


		//_CrtDumpMemoryLeaks();

	}
	SECTION("parse list of expressions")
	{
		string input = "a(b,c,d,)";
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
		REQUIRE(tree.first.isSet() == true);
	}

	for (std::size_t i = 0; i < input.size(); ++i)
	{
		if (i > 0 && i < input.size() - 1 && input[i] == ','
			&& std::isalpha(input[i - 1]) && std::isalpha(input[i + 1])) {
			continue;
		}
		std::string modifiedInput = input.substr(0, i) + input.substr(i + 1);
		tree = parseExpression(modifiedInput);
		REQUIRE(tree.first.isSet() == true);
	}
	
	// major issues
	
	string input22 = "(a,b,c,)";
	tree = parseExpression(input22);
	REQUIRE(tree.first.isSet() == true);

	
	// does not pass

	string input2 = "a(a())";
	tree = parseExpression(input2);
	tree.first.message();
	REQUIRE(tree.first.isSet() == true);
	
	
}

TEST_CASE("Manual Tests")
{
	pair<ParseError, ExpressionTreeNode> tree;
	SECTION("Error Incorrect Comma Location - Variable")
	{
		string input2 = "a(X(,))";
		tree = parseExpression(input2);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
	
	SECTION("Missmatche Parenthesis - Variable")
	{
		string input2 = "a(X())";
		tree = parseExpression(input2);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
	
	SECTION("Missing Comma - Variable")
	{
		string input2 = "a(X y)";
		tree = parseExpression(input2);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
	
	SECTION("Invalid argument type - Variable")
	{
		string input2 = "a(2,y)";
		tree = parseExpression(input2);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
	SECTION("Invalid argument type - Variable")
	{
		string input2 = "f(a,b";
		tree = parseExpression(input2);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
}

TEST_CASE("Parse Query Tests")
{
	pair<ParseError, ExpressionTreeNode> tree;
	SECTION("Atom only")
	{
		string input = "f.";
		tree = parseQuery(input);
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "a(b) . ";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		tree = parseQuery(t1);
		REQUIRE(tree.first.isSet() == false);
	}

	SECTION("unary predicate - fact")
	{
		string input = "a(X) . ";
		tree = parseQuery(input);
		REQUIRE(tree.first.isSet() == false);
	}
	SECTION("binary predicate - variable")
	{
		string input = "g(b,c). ";
		tree = parseQuery(input);
		tree.first.message();
		REQUIRE(tree.first.isSet() == false);
	}
	SECTION("binary predicate - variable")
	{
		string input = "a.)";
		tree = parseQuery(input);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}
	SECTION("binary predicate - variable")
	{
		string input = "a.)";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		tree = parseQuery(t1);
		tree.first.message();
		REQUIRE(tree.first.isSet() == true);
	}

	SECTION("arbitrary tree 1")
	{
		string input = "a(b(c,d),c,e(f,g,h)) .";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		tree = parseQuery(t1);
		REQUIRE(tree.first.isSet() == false);
	}

	

}