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
		//REQUIRE(result.substitution.lookup(y).front().contents == x.contents);
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
		//REQUIRE(result.substitution.lookup(X).front() == a);
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
		//REQUIRE(result.substitution.lookup(X).front() == a);
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
		//REQUIRE(result.substitution.lookup(makeVariable("X")).front() == makeAtom("a"));
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
		//REQUIRE(result.substitution.lookup(makeVariable("X")).front() == makeAtom("a"));
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
		//REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "g(x)");
		REQUIRE(result.substitution.data.size() == 2);
	}

	SECTION("unify atom facts", "[unify]")
	{
		// Test case for unifying two atoms that are equal
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(mary, pizza)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(mary, pizza)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 0); // No substitutions expected
	}
	SECTION("unify unary predicate facts", "[unify]") {
		pair<ParseError, ExpressionTreeNode>  tree1 = parseExpression("likes(mary)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(mary)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 0); // No substitutions expected
	}

	SECTION("unify binary predicate facts", "[unify]") {
		pair<ParseError, ExpressionTreeNode>  tree1 = parseExpression("likes(mary,pizza)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(mary,pizza)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 0); // No substitutions expected
	}

	SECTION("unify unary predicate with variables", "[unify]") {
		pair<ParseError, ExpressionTreeNode>  tree1 = parseExpression("likes(X)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(pizza)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 1);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "pizza");
	}

	SECTION("unify binary predicate with variables", "[unify]") {
		pair<ParseError, ExpressionTreeNode>  tree1 = parseExpression("likes(X, Y)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(mary, pizza)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 2);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "mary");
		REQUIRE(result.substitution.lookup(makeVariable("Y")).front().toString() == "pizza");
	}

	// ... Add more test cases for other scenarios ...

	SECTION("Test failure: multiple assignment to variable", "[unify]") {
		pair<ParseError, ExpressionTreeNode>  tree1 = parseExpression("likes(X, pizza)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(mary, pizza)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 1);
	}

	SECTION("Test failure: mismatched compounds", "[unify]") {
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(mary, pizza)");
		pair<ParseError, ExpressionTreeNode>  tree2 = parseExpression("likes(mary)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == true);
		REQUIRE(result.substitution.data.size() == 0);
	}

	SECTION("test unify variable multiple times", "[unify]") {
		// Test case for multiple assignments to the same variable
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(X,pasta)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(greens,X)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == true);
	}
	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(X,Y)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(Y,pasta)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "Y");
		REQUIRE(result.substitution.lookup(makeVariable("Y")).front().toString() == "pasta");
	}
	SECTION("test unify variable multiple times", "[unify]") {
		// Test case for multiple assignments to the same variable
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(X,X)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(greens,greens)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).size() == 1);
	}
	SECTION("test unify variable multiple times", "[unify]") {
		// Test case for multiple assignments to the same variable
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(X,Y)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(greens,greens)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).size() == 1);
	}
	SECTION("test complex nested structures", "[unify]") {
		// Test case for complex nested structures
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("f(g(X), h(Y, k(a)))");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("f(g(Z), h(k(Z), k(a)))");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X")).front().toString() == "Z");
		REQUIRE(result.substitution.lookup(makeVariable("Y")).front().toString() == "k(Z)");
	}
	SECTION("test unify variable multiple times with failure", "[unify]") {
		// Test case for multiple assignments to the same variable
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("likes(g(X),X)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("likes(g(a),a)");

		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.data.size() == 1);
	}
	SECTION("test unify variable multiple times", "[unify]") {
		// Test case for multiple assignments to the same variable
		pair<ParseError, ExpressionTreeNode> tree1 = parseExpression("f(a,X1)");
		pair<ParseError, ExpressionTreeNode> tree2 = parseExpression("f(Y1,a)");
		makeVariable("X1");
		UnificationResult result;
		vtpl::unify(tree1.second, tree2.second, result);

		REQUIRE(result.failed == false);
		REQUIRE(result.substitution.lookup(makeVariable("X1")).front() == makeAtom("a"));
		REQUIRE(result.substitution.lookup(makeVariable("Y1")).front() == makeAtom("a"));
		REQUIRE(result.substitution.lookup(makeVariable("Y1")).size() == 1);
		REQUIRE(result.substitution.lookup(makeVariable("X1")).size() == 1);
	}
}