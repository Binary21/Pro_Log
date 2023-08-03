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

	

}