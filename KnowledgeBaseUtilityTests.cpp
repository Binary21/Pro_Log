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
	SECTION("Test substitution insert and loop")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		REQUIRE(subst.lookup(makeVariable("X")).front().toString() == "a");
	}
	SECTION("Variable to Atom") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		ExpressionTreeNode t3 = apply(t1, subst);
		REQUIRE(t3.toString() == "a");
	}
	SECTION("Variable to Compound") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeCompound("g", { makeAtom("a") }));
		ExpressionTreeNode t3 = apply(t1, subst);
		REQUIRE(t3.toString() == "g(a)");
	}
	SECTION("Variable to Variable") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeVariable("Y"));
		ExpressionTreeNode t3 = apply(t1, subst);
		REQUIRE(t3.toString() == "Y");
	}
	SECTION("Test apply with expression list") {
		list<ExpressionTreeNode> list_ = { makeVariable("X"), makeVariable("Y") };
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		subst.insert(makeVariable("Y"), makeAtom("b"));
		list<ExpressionTreeNode> result;
		std::transform(list_.begin(), list_.end(), std::back_inserter(result),
			[&](const ExpressionTreeNode& t) { return apply(t, subst); });
		auto it = result.begin();
		REQUIRE(it->toString() == "a");
		it++;
		REQUIRE(it->toString() == "b");
	}
}
TEST_CASE("APPLY")
{
	SECTION("Apply")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));

		string input = "f(X)";
		pair<ParseError, ExpressionTreeNode> root;
		root = parseExpression(input);

		ExpressionTreeNode result;
		result = apply(root.second, subst);
		REQUIRE(result.toString() == "(f(a))");
	}
	SECTION("Apply 2")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeCompound("g", { makeAtom("a") }));

		string input = "f(X)";
		pair<ParseError, ExpressionTreeNode> root;
		root = parseExpression(input);

		ExpressionTreeNode result;
		result = apply(root.second, subst);
		REQUIRE(result.toString() == "(f(g(a)))");
	}
	SECTION("Apply 2")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeVariable("Y"));

		string input = "f(X)";
		pair<ParseError, ExpressionTreeNode> root;
		root = parseExpression(input);

		ExpressionTreeNode result;
		result = apply(root.second, subst);
		REQUIRE(result.toString() == "(f(Y))");
	}
	// this problem

	SECTION("Applying substitution to expression list - further")
	{
		UnificationResult subst;

		pair<ParseError, ExpressionTreeNode> root;
		string input = "f(X),g(Y)";
		pair<ParseError, ExpressionTreeNode> root2;
		string input2 = "f(a),g(b)";

		root2 = parseExpression(input2);
		root = parseExpression(input);

		unify(root.second, root2.second, subst);

		ExpressionTreeNode result;
		result = apply(root.second, subst.substitution);
		REQUIRE(result.toString() == "(f(a),g(b))");
	}
	SECTION("Applying substitution to expression list - further")
	{
		UnificationResult subst;

		ExpressionTreeNode X = makeVariable("X");
		ExpressionTreeNode Y = makeVariable("Y");
		ExpressionTreeNode f1 = makeCompound("f", { X });
		ExpressionTreeNode g1 = makeCompound("g", { Y });

		ExpressionTreeNode none1;
		none1.children = { f1, g1 };

		ExpressionTreeNode a = makeAtom("a");
		ExpressionTreeNode b = makeAtom("b");
		ExpressionTreeNode f2 = makeCompound("f", { a });
		ExpressionTreeNode g2 = makeCompound("g", { b });

		ExpressionTreeNode none2;
		none2.children = { f2, g2 };

		unify(none1, none2, subst);

		ExpressionTreeNode result;
		result = apply(none1, subst.substitution);
		REQUIRE(result.toString() == "(f(a),g(b))");
	}
	SECTION("Applying substitution to expression list - without root node")
	{

		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		subst.insert(makeVariable("Y"), makeAtom("b"));

		ExpressionTreeNode X = makeVariable("X");
		ExpressionTreeNode Y = makeVariable("Y");
		ExpressionTreeNode f = makeCompound("f", { X });
		ExpressionTreeNode g = makeCompound("g", { Y });
		ExpressionTreeNode root;
		root.children = { f, g };
		ExpressionTreeNode result;
		result = apply(root, subst);
		REQUIRE(result.toString() == "(f(a),g(b))");
	}
	// 
	SECTION("Unifying two incompatible lists")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		subst.insert(makeVariable("Y"), makeAtom("b"));

		pair<ParseError, ExpressionTreeNode> root1;
		pair<ParseError, ExpressionTreeNode> root2;
		string input1 = "f(X). g(Y).";
		string input2 = "f(a). g(b).";
		root1 = parseExpression(input1);
		root2 = parseExpression(input2);

		ExpressionTreeNode result;
		result = apply(root1.second, subst);

		REQUIRE(result.toString() == root2.second.toString());
	}

	SECTION("Compose test")
	{
		Substitution subst1;
		subst1.insert(makeVariable("X"), makeAtom("a"));
		Substitution subst2;
		subst2.insert(makeVariable("Y"), makeAtom("b"));

		pair<ParseError, ExpressionTreeNode> tree;
		string treeInput = "f(X, Y)";

		tree = parseExpression(treeInput);
		ExpressionTreeNode test = apply(tree.second, subst1);
		REQUIRE(apply(tree.second, compose(subst1, subst2)) == apply(apply(tree.second, subst1), subst2));
	}
	SECTION("Compose test")
	{
		Substitution subst1;
		subst1.insert(makeVariable("X"), makeAtom("a"));
		Substitution subst2;
		subst2.insert(makeVariable("Y"), makeAtom("b"));

		pair<ParseError, ExpressionTreeNode> tree;
		string treeInput = "f(X,g(a),a,g(c,g),g(Y,X),Y)";

		tree = parseExpression(treeInput);
		ExpressionTreeNode test = apply(tree.second, subst1);
		REQUIRE(apply(tree.second, compose(subst1, subst2)) == apply(apply(tree.second, subst1), subst2));
	}
	SECTION("Compose test")
	{
		Substitution subst1;
		Substitution subst2;

		subst1.insert(makeVariable("X"), makeVariable("Y"));
		subst1.insert(makeVariable("Z"), makeAtom("b"));
		subst2.insert(makeVariable("Y"), makeAtom("a"));

		pair<ParseError, ExpressionTreeNode> tree;
		string treeInput = "f(X,Y,Z)";

		tree = parseExpression(treeInput);

		ExpressionTreeNode test = apply(tree.second, subst1);
		REQUIRE(apply(test, subst2).toString() == "(f(a,a,b))");
	}
}
TEST_CASE("Test unification with expression list failure", "[unification]") {
	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> exp1 = parseExpression("f(a, b)");
		pair<ParseError, ExpressionTreeNode> exp2 = parseExpression("f(X, c)");

		vtpl::UnificationResult result;
		vtpl::unify(exp1.second, exp2.second, result);

		REQUIRE(result.failed == true);
	}
	SECTION("")
	{
		pair<ParseError, ExpressionTreeNode> exp1 = parseExpression("f(a),g(b)");
		pair<ParseError, ExpressionTreeNode> exp2 = parseExpression("f(X)");

		vtpl::UnificationResult result;
		vtpl::unify(exp1.second, exp2.second, result);

		REQUIRE(result.failed == true);
	}
	SECTION("Applying substitution to expression list - failure")
	{
		vtpl::UnificationResult subst;

		std::pair<ParseError, ExpressionTreeNode> root1;
		std::pair<ParseError, ExpressionTreeNode> root2;

		std::string input1 = "X";
		std::string input2 = "f(X)";

		root1 = parseExpression(input1);
		root2 = parseExpression(input2);

		unify(root1.second, root2.second, subst);
		REQUIRE(subst.failed == true);
	}
	SECTION("composition test")
	{
		Substitution subst1;
		Substitution subst2;
		Substitution subst;

		subst1.insert(makeVariable("X"), makeVariable("Y"));
		subst2.insert(makeVariable("Y"), makeAtom("a"));

		subst = compose(subst1, subst2);

		REQUIRE(subst.data.size() == 2);
		REQUIRE(subst.lookup(makeVariable("X")).front() == makeAtom("a"));
		REQUIRE(subst.lookup(makeVariable("Y")).front() == makeAtom("a"));
	}

	SECTION("Multiple substitutions for same variable and multiple occurrences in expression")
	{
		Substitution subst1, subst2;
		subst1.insert(makeVariable("X"), makeAtom("bill"));
		subst2.insert(makeVariable("X"), makeAtom("sally"));

		string input = "friends(X, X)";
		pair<ParseError, ExpressionTreeNode> root;
		root = parseExpression(input);

		// Apply subst1
		ExpressionTreeNode result1;
		result1 = apply(root.second, subst1);
		REQUIRE(result1.toString() == "(friends(bill,bill))");

		// Apply subst2
		ExpressionTreeNode result2;
		result2 = apply(root.second, subst2);
		REQUIRE(result2.toString() == "(friends(sally,sally))");
	}
}