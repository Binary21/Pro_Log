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
	SECTION("")
	{
		string input1 = "a";
		string input2 = "X";
		ExpressionTreeNode x = makeAtom(input1);
		ExpressionTreeNode y = makeVariable(input2);
		UnificationResult result;
		vtpl::unify(x, y, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(y).front().contents == x.contents);
	}
	SECTION("")
	{
		string input1 = "a";
		string input2 = "X";
		string input3 = "f";
		ExpressionTreeNode a = makeAtom(input1);
		ExpressionTreeNode X = makeVariable(input2);

		ExpressionTreeNode f1 = makeCompound(input3, { a });
		ExpressionTreeNode f2 = makeCompound(input3, { X });

		UnificationResult result;
		vtpl::unify(f1, f2, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(X).front() == a);
	}
	SECTION("")
	{
		string input1 = "a";
		string input2 = "X";
		string input3 = "f";
		ExpressionTreeNode a = makeAtom(input1);
		ExpressionTreeNode X = makeVariable(input2);

		ExpressionTreeNode f1 = makeCompound(input3, { a });
		ExpressionTreeNode f2 = makeCompound(input3, { X });

		UnificationResult result;
		vtpl::unify(f1, f2, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(X).front() == a);
	}

	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> tree1;
		pair<ParseError, ExpressionTreeNode> tree2;
		string treeInput1 = "f(a)";
		string treeInput2 = "f(X)";
		tree1 = parseExpression(treeInput1);
		tree2 = parseExpression(treeInput2);

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front() == makeAtom("a"));
	}

	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> tree1;
		pair<ParseError, ExpressionTreeNode> tree2;
		string treeInput1 = "f(a,Y)";
		string treeInput2 = "f(X,c)";
		tree1 = parseExpression(treeInput1);
		tree2 = parseExpression(treeInput2);

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front() == makeAtom("a"));
		REQUIRE(result.substitution.data.size() == 2);
	}
	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> tree1;
		pair<ParseError, ExpressionTreeNode> tree2;
		string treeInput1 = "f(g(x),Y)";
		string treeInput2 = "f(X,c)";
		tree1 = parseExpression(treeInput1);
		tree2 = parseExpression(treeInput2);

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);
		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "g(x)");
		REQUIRE(result.substitution.data.size() == 2);
	}
}