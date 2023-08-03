#include "catch.hpp"
#include "KnowledgeBaseUtility.hpp"
#include "Unification.hpp"
#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"
#include "KnowledgeBase.hpp"
#include <sstream>
#include <string>
#include "test_config.hpp"
#include <fstream>

using namespace vtpl;
using namespace std;

TEST_CASE("")
{
	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> tree1;
		pair<ParseError, ExpressionTreeNode> tree2;
		pair<ParseError, ExpressionTreeNode> tree3;
		string treeInput1 = "f(X, a)";
		string treeInput2 = "f(b, a)";

		tree1 = parseExpression(treeInput1);
		tree2 = parseExpression(treeInput2);

		UnificationResult result;
		unify(tree1.second, tree2.second, result);
		REQUIRE(result.failed == false);

		tree3.second = apply(tree1.second, result.substitution);
		REQUIRE(tree3.second.toString() == "(f(b,a))");
	}
	
}