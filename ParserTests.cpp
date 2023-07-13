#include "catch.hpp"

#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"

#include <sstream>
#include <string>

using namespace std;
using namespace vtpl;

TEST_CASE("")
{
	SECTION("")
	{
		string input = "bob()";
		istringstream iss(input);

		TokenList t1 = tokenize(iss);
		pair<ParseError, ExpressionTreeNode> error = parseExpression(t1);
		error.first.isSet();
		error.first.message();
		//ExpressionTreeNode tree = parseExpression(t1);
		//tree.toString();
		REQUIRE(true);

	}
}