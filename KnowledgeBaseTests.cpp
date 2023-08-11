//=== ExpressionTreeTests.cpp - Catch tests of tokenize implementation
// 
//=================//
#include "catch.hpp"

#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"
#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"
#include <sstream>
#include <string>
#include "test_config.hpp"
#include <fstream>

using namespace vtpl;
using namespace std;
TEST_CASE("Parse KnowledgeBase")
{
	SECTION("Correct input - tokens")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). ";
		istringstream iss(input);
		TokenList t1 = tokenize(iss);
		knowledgeBase = parseKnowledgeBase(t1);
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Correct input - string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). ";
		knowledgeBase = parseKnowledgeBase(input);
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Correct input - string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). \n b(X,Y) :- z(X),q(b,Y). \n g(z,Q) :- g(z(q),X).";
		knowledgeBase = parseKnowledgeBase(input);
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
		REQUIRE(isCompound(std::get<1>(knowledgeBase).begin()->head));
	}
	SECTION("multiple simple facts")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "f.";
		knowledgeBase = parseKnowledgeBase(input);
		REQUIRE(isAtom(std::get<1>(knowledgeBase).begin()->head));
	}

	SECTION("Parsing basic knowledge base - multiple simple facts")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "f. \n g. \n h.";
		knowledgeBase = parseKnowledgeBase(input);

		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		REQUIRE(std::get<1>(knowledgeBase).size() == 3);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "f");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "g");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "h");
		REQUIRE(isNone(it->body));
	}

	SECTION("Parsing basic knowledge base - a single, unary predicate fact")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(X).";
		knowledgeBase = parseKnowledgeBase(input);

		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "likes(X)");
		REQUIRE(isNone(it->body));
	}

	SECTION("Parsing basic knowledge base - a single, binary predicate fact")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(X,Y).";
		knowledgeBase = parseKnowledgeBase(input);

		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		REQUIRE(std::get<1>(knowledgeBase).size() == 1);
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "likes(X,Y)");
		REQUIRE(isNone(it->body));
	}

	SECTION("Parsing basic knowledge base - single implication with a fact body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :- q.";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());


		REQUIRE(std::get<1>(knowledgeBase).size() == 1);


		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "p");
		REQUIRE(it->body.toString() == "(q)");
	}

	SECTION("Parsing simple likes knowledge base")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(john, pizza). likes(mary, ice_cream).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());


		REQUIRE(std::get<1>(knowledgeBase).size() == 2);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "likes(john,pizza)");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(mary,ice_cream)");
		REQUIRE(isNone(it->body));
	}

	SECTION("Expected parsing errors - Incomplete predicate list, missing comma")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X Y).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Expected parsing errors - Incomplete predicate list, comma at end")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y,), q(Z).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Expected parsing errors - missing full stop")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y)";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Test parse KB from string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :- q(X, Y).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());


		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "p");
		REQUIRE(it->body.toString() == "(q(X,Y))");
	}

	SECTION("Error in clause head")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(, X, Y).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Error: missing clause body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :-";
		knowledgeBase = parseKnowledgeBase(input);

		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}


	SECTION("Error: in clause body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y) :- q(X,, Y).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}


	SECTION("Error: invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p() :- q().";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Parse simple likes knowledge base")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(john, pizza). likes(mary, ice_cream). likes(anna, sushi).";
		knowledgeBase = parseKnowledgeBase(input);

		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());


		REQUIRE(std::get<1>(knowledgeBase).size() == 3);


		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "likes(john,pizza)");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(mary,ice_cream)");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(anna,sushi)");
		REQUIRE(isNone(it->body));
	}

	SECTION("Invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = " :- g(z,k).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Head made up of expression list")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "g(p,k),g(q,z) :- g(z,k).";
		knowledgeBase = parseKnowledgeBase(input);


		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).  \n likes(bill, movies). \n likes(sally, movies). \n likes(bob, pizza).";
		knowledgeBase = parseKnowledgeBase(input);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "friends(X,Y)");
		REQUIRE(it->body.toString() == "(likes(X,Z),likes(Y,Z))");

		++it;
		REQUIRE(it->head.toString() == "likes(bill,movies)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(sally,movies)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(bob,pizza)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
	SECTION("")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = TEST_FILE_DIR + "/likes.pro";
		ifstream likes(input);
		TokenList t1 = tokenize(likes);
		knowledgeBase = parseKnowledgeBase(t1);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "friends(X,Y)");
		REQUIRE(it->body.toString() == "(likes(X,Z),likes(Y,Z))");

		++it;
		REQUIRE(it->head.toString() == "likes(bill,movies)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(sally,movies)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "likes(bob,pizza)");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
}
TEST_CASE("Ask tests")
{
	SECTION("Testing ask on kb, seeking friends")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "";
		kb = parseKnowledgeBase(input);

		ExpressionTreeNode query = parseExpression("friends(sally,X)").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(result.empty());
	}
	SECTION("Testing ask on small kb, expect failure.")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "likes(bill,movies).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("likes(sally,movies)").second;
		list<Substitution> result = std::get<1>(kb).ask(query);
		REQUIRE(result.empty());
	}
	SECTION("Testing ask on tiny kb with rule")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z). likes(bill,movies). likes(sally,movies).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("friends(X,Y)").second;
		list<Substitution> result = std::get<1>(kb).ask(query);
		for (Substitution subst : result)
		{
			ExpressionTreeNode application = apply(query, subst);
		}
	}
	SECTION("Testing ask on tiny kb of facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "likes(bill, X). likes(sally, Y).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("likes(X,Y)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}
	SECTION("Testing ask on tiny kb of facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "likes(bill, movies). likes(sally, movies).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("likes(X,movies)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}
	SECTION("Testing ask on a single fact kb")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(a).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}
	SECTION("Testing ask on two-argument fact kb")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "g(b,c).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("g(X,Y)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}

	SECTION("Testing ask on multi-fact kb")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "h(i,j). k(l).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("h(X,Y)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}

	SECTION("Testing ask on non-existing fact kb")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "m(n).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("m(X,Y)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(result.empty());
	}
	SECTION("Testing ask on empty kb")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(result.empty());
	}
	SECTION("Testing ask with kb of single variable facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X). ";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}

	SECTION("Testing ask with kb of multi-variable facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X, Y, Z). ";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X, Y, Z)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}

	SECTION("Testing ask with kb of repeated facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X). f(X). ";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}

	SECTION("Testing ask with kb of different facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X). g(X). ";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(X)").second;
		auto result = get<1>(kb).ask(query);

		REQUIRE(!result.empty());
	}
	SECTION("All arguments in facts are variables")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X, Y).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(g(a), a)").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(!result.empty());
	}
	SECTION("Goal doesn't match with any fact")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(a, b).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("g(a, b)").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(result.empty());
	}
	SECTION("Goal has more arguments than the facts")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(a).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(a, b)").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(result.empty());
	}
	SECTION("Facts contain variables, but goal doesn't provide any substitution")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(a, b)").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(result.empty());
	}
	SECTION("Facts contain variables, but goal doesn't provide any substitution")
	{
		tuple<ParseError, KnowledgeBase> kb;
		string input = "f(X) :- g(X). f(a). g(a).";
		kb = parseKnowledgeBase(input);
		ExpressionTreeNode query = parseExpression("f(a).").second;
		auto result = get<1>(kb).ask(query);
		REQUIRE(!result.empty());
		REQUIRE(result.size() == 2);
	}
}

TEST_CASE("Unionize")
{
	list<Substitution> s;
	list<Substitution> s2;
	list<Substitution> result;

	Substitution subst1;
	Substitution subst2;
	Substitution subst3;
	Substitution subst4;
	Substitution subst5;
	Substitution subst6;
	Substitution subst7;

	subst1.insert(makeVariable("X"), makeAtom("l"));
	subst1.insert(makeVariable("Y"), makeAtom("a"));


	subst3.insert(makeVariable("Y"), makeAtom("a"));

	subst2.insert(makeVariable("Z"), makeAtom("g"));
	subst4.insert(makeVariable("Q"), makeAtom("q"));
	subst5.insert(makeVariable("G"), makeAtom("l"));
	subst6.insert(makeVariable("B"), makeAtom("z"));
	subst7.insert(makeVariable("N"), makeAtom("w"));

	s.push_back(subst1);
	s.push_back(subst2);
	s.push_back(subst7);
	s.push_back(subst4);

	s2.push_back(subst3);
	s2.push_back(subst4);
	s2.push_back(subst5);
	s2.push_back(subst6);
	s2.push_back(subst7);

	result = unionize(s2, s);
	REQUIRE(true);
}