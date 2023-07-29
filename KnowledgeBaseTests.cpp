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

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 3);

		// Verify the clauses and their contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(f)");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(g)");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(h)");
		REQUIRE(isNone(it->body));
	}
	
	SECTION("Parsing basic knowledge base - a single, unary predicate fact")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(X).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		// Verify the clause and its contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(likes(X))");
		REQUIRE(isNone(it->body));
	}
	
	SECTION("Parsing basic knowledge base - a single, binary predicate fact")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(X,Y).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		// Verify the clause and its contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(likes(X,Y))");
		REQUIRE(isNone(it->body));
	}
	
	SECTION("Parsing basic knowledge base - single implication with a fact body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :- q.";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		// Verify the clause and its contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(p)");
		REQUIRE(it->body.toString() == "(q)");
	}
	
	SECTION("Parsing simple likes knowledge base")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(john, pizza). likes(mary, ice_cream).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 2);

		// Verify the clauses and their contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(likes(john,pizza))");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(mary,ice_cream))");
		REQUIRE(isNone(it->body));
	}
	
	SECTION("Expected parsing errors - Incomplete predicate list, missing comma")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X Y).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}
	
	SECTION("Expected parsing errors - Incomplete predicate list, comma at end")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y,), q(Z).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}
	
	SECTION("Expected parsing errors - missing full stop")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y)";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}
	
	SECTION("Test parse KB from string")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :- q(X, Y).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 1);

		// Verify the clause and its contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(p)");
		REQUIRE(it->body.toString() == "(q(X,Y))");
	}

	// Additional Test Cases

	// Test parsing errors for invalid head clause
	SECTION("Error in clause head")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(, X, Y).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	// Test parsing errors for missing clause body
	SECTION("Error: missing clause body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p :-";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	// Test parsing errors in clause body
	SECTION("Error: in clause body")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p(X, Y) :- q(X,, Y).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	// Test parsing errors for invalid head clause
	SECTION("Error: invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "p() :- q().";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}
	SECTION("Parse simple likes knowledge base")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "likes(john, pizza). likes(mary, ice_cream). likes(anna, sushi).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser doesn't return any errors
		REQUIRE_FALSE(std::get<0>(knowledgeBase).isSet());

		// Verify the number of clauses in the knowledge base
		REQUIRE(std::get<1>(knowledgeBase).size() == 3);

		// Verify the clauses and their contents
		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(likes(john,pizza))");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(mary,ice_cream))");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(anna,sushi))");
		REQUIRE(isNone(it->body));
	}	

	SECTION("Invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = " :- g(z,k).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Head made up of expression list")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "g(p,k),g(q,z) :- g(z,k).";
		knowledgeBase = parseKnowledgeBase(input);

		// Verify that the parser returns the expected parsing error
		REQUIRE(std::get<0>(knowledgeBase).isSet());
	}

	SECTION("Invalid head clause")
	{
		tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
		string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).  \n likes(bill, movies). \n likes(sally, movies). \n likes(bob, pizza).";
		knowledgeBase = parseKnowledgeBase(input);

		vtpl::KnowledgeBase::Iterator it = std::get<1>(knowledgeBase).begin();
		REQUIRE(it->head.toString() == "(friends(X,Y))");
		REQUIRE(it->body.toString() == "(likes(X,Z),likes(Y,Z))");

		++it;
		REQUIRE(it->head.toString() == "(likes(bill,movies))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(sally,movies))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(bob,pizza))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));
		// Verify that the parser returns the expected parsing error
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
		REQUIRE(it->head.toString() == "(friends(X,Y))");
		REQUIRE(it->body.toString() == "(likes(X,Z),likes(Y,Z))");

		++it;
		REQUIRE(it->head.toString() == "(likes(bill,movies))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(sally,movies))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));

		++it;
		REQUIRE(it->head.toString() == "(likes(bob,pizza))");
		REQUIRE(it->body.toString() == "");
		REQUIRE(isNone(it->body));
		// Verify that the parser returns the expected parsing error
		REQUIRE(!std::get<0>(knowledgeBase).isSet());
	}
}