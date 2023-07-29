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
		REQUIRE(tree3.second.toString() == "f(b,a)");
	}
	SECTION("")
	{
		tuple<ParseError, KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). g(X,Y) :- b(Z,R).";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		knowledgeBase = parseKnowledgeBase(t1);
		Clause result;
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		result = apart(*it);
		REQUIRE(result.head.toString() == "h(X_1,Y_1)");
		REQUIRE(result.body.toString() == "(f(X_1),g(b,Y_1))");
		it++;
		
		result = apart(*it);
		REQUIRE(result.head.toString() == "g(X_2,Y_2)");
		REQUIRE(result.body.toString() == "(b(Z_1,R_1))");
		
		
	}
	
	SECTION("")
	{
		tuple<ParseError, KnowledgeBase> knowledgeBase;
		string input = "h(X,X).";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		knowledgeBase = parseKnowledgeBase(t1);
		Clause result;
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		result = apart(*it);
		REQUIRE(result.head.toString() == "h(X_3,X_3)");
	}

	SECTION("")
	{
		Substitution s1;
		s1.insert(makeVariable("X"), makeAtom("a"));
		s1.insert(makeVariable("Y"), makeAtom("b"));

		Substitution s2;
		s1.insert(makeAtom("X"), makeAtom("c"));
		s1.insert(makeAtom("Y"), makeAtom("d"));

		Substitution result = compose(s1, s2);

		Substitution expected;
		expected.insert(makeVariable("X"), makeAtom("c"));
		expected.insert(makeVariable("Y"), makeAtom("d"));

		REQUIRE(result.data == expected.data);
	}
}