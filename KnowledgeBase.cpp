#include <string>
#include <iostream>
#include "KnowledgeBase.hpp"
#include "catch.hpp"

using namespace std;

void vtpl::KnowledgeBase::tell(const Clause& clause)
{
	clauses.push_back(clause);
}

size_t vtpl::KnowledgeBase::size() const
{
	return clauses.size();
}

vtpl::KnowledgeBase::Iterator vtpl::KnowledgeBase::begin() const
{
	
	return clauses.begin();
}

vtpl::KnowledgeBase::Iterator vtpl::KnowledgeBase::end() const
{
	return clauses.end;
}
