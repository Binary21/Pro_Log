//=== UnificationTests.cpp - Catch tests of tokenize implementation
// 
//=================//
#include "catch.hpp"
#include "Parser.hpp"
#include "Unification.hpp"
#include "ExpressionTree.hpp"
#include <sstream>
#include <string>


using namespace vtpl;
using namespace std;

TEST_CASE("")
{
	SECTION("")
	{
		string input = "a";
		ExpressionTreeNode x = makeAtom(input);
		ExpressionTreeNode y = makeAtom(input);
		UnificationResult result;
		vtpl::unify(x, y, result);
		REQUIRE(result.failed == false);
	}
	
}
