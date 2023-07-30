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
		//REQUIRE(result.failed == false);

		tree3.second = apply(tree1.second, result.substitution);
		//REQUIRE(tree3.second.toString() == "(f(b,a))");
	}
	SECTION("Test substitution insert and loop")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		//REQUIRE(subst.lookup(makeVariable("X")).front().toString() == "a");
	}
	SECTION("Test substitution iterators")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		subst.insert(makeVariable("Y"), makeAtom("b"));
		auto it = subst.constBegin();
		//REQUIRE(it->first.toString() == "X");
		//REQUIRE(it->second.toString() == "a");
		it++;
		//REQUIRE(it->first.toString() == "Y");
		//REQUIRE(it->second.toString() == "b");

	}
	SECTION("Variable to Atom") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		ExpressionTreeNode t3 = apply(t1, subst);
		//REQUIRE(t3.toString() == "(a)");
	}
	SECTION("Variable to Compound") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeCompound("g", { makeAtom("a") }));
		ExpressionTreeNode t3 = apply(t1, subst);
		//REQUIRE(t3.toString() == "(g(a))");
	}
	SECTION("Variable to Variable") {
		ExpressionTreeNode t1 = makeVariable("X");
		Substitution subst;
		subst.insert(makeVariable("X"), makeVariable("Y"));
		ExpressionTreeNode t3 = apply(t1, subst);
		//REQUIRE(t3.toString() == "(Y)");
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
		//REQUIRE(it->toString() == "(a)");
		it++;
		//REQUIRE(it->toString() == "(b)");
	}
	SECTION("Test standardize apart (and apply)") {
		ExpressionTreeNode t1 = makeCompound("f", { makeVariable("X") });
		cout << t1.toString() << endl;
		SubstitutionData substData;
		standardizeApart(t1, substData);
		//REQUIRE(t1.toString() == "f(X_1)");
	}
	SECTION("Applying substitution to expression list - further")
	{
		Substitution subst;
		subst.insert(makeVariable("X"), makeAtom("a"));
		subst.insert(makeVariable("Y"), makeAtom("b"));

		pair<ParseError, ExpressionTreeNode> root;
		string input = "f(X),g(Y)";
		root = parseExpression(input);
		ExpressionTreeNode result;
		result = apply(root.second, subst);
		//REQUIRE(result.toString() == "(f(a),g(b))");
	}
}