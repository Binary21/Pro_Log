//=== Lexer.hpp - Lexer class definition ==============================//
//
// \file
// This file contains the definition of the Lexer class
//
//
//=====================================================================//

#ifndef KNOWLEDGEBASE_HPP
#define KNOWLEDGEBASE_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Lexer.hpp"
#include "ExpressionTree.hpp"
#include "Token.hpp"

using namespace std;
namespace vtpl {


struct Clause
{
	ExpressionTreeNode head;
	ExpressionTreeNode body;

};

class KnowledgeBase
{
public:
	typedef std::vector<Clause>::const_iterator Iterator;

	// add a clause to the database
	void tell(const Clause& clause);

	// return the number of clauses in the database
	size_t size() const; 

	// get an interator to the first clause
	Iterator begin() const;

	// get an iterator one-past the last caluse
	Iterator end() const;

private:
	vector<Clause>clauses;
};
};

#endif