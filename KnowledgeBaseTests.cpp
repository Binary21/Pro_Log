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
}