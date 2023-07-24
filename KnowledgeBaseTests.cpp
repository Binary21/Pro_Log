//=== ExpressionTreeTests.cpp - Catch tests of tokenize implementation
// 
//=================//
#include "catch.hpp"

#include "Parser.hpp"
#include "ExpressionTree.hpp"
#include "Lexer.hpp"
#include "KnowledgeBase.hpp"
#include <sstream>
#include <string>

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
		cout << "head: " << std::get<1>(knowledgeBase).begin()->head.toString() << " body: " << std::get<1>(knowledgeBase).begin()->body.toString() << endl;
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Correct input - string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). ";
		knowledgeBase = parseKnowledgeBase(input);
		cout << "head: " << std::get<1>(knowledgeBase).begin()->head.toString() << " body: " << std::get<1>(knowledgeBase).begin()->body.toString() << endl;
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Correct input - string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "h(X,Y) :- f(X),g(b,Y). \n b(X,Y) :- z(X),q(b,Y). \n g(z,Q) :- g(z(q),X).";
		knowledgeBase = parseKnowledgeBase(input);
		for (vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin(); it != std::get<1>(knowledgeBase).end(); ++it)
		{
			cout << "head: " << it->head.toString() << " body: " << it->body.toString() << endl;
		}
		//cout << "head: " << std::get<1>(knowledgeBase).begin()->head.toString() << " body: " << std::get<1>(knowledgeBase).begin()->body.toString() << endl;
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
		REQUIRE(isCompound(std::get<1>(knowledgeBase).begin()->head));
	}
	SECTION("multiple simple facts")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "f.";
		knowledgeBase = parseKnowledgeBase(input);
		cout << "head: " << std::get<1>(knowledgeBase).begin()->head.toString() << " body: " << std::get<1>(knowledgeBase).begin()->body.toString() << endl;
		REQUIRE(isAtom(std::get<1>(knowledgeBase).begin()->head));
	}
}