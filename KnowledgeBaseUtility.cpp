#include "KnowledgeBaseUtility.hpp"

using namespace vtpl;
using namespace std;

ExpressionTreeNode vtpl::applyHelper(const ExpressionTreeNode& t, const Substitution& sub)
{
	if (sub.lookup(t).empty())
		return t;
	return t;
}

ExpressionTreeNode vtpl::apply(const ExpressionTreeNode& t, const Substitution& sub)
{
	return (applyHelper(t, sub));
}

void vtpl::standardizeApart(ExpressionTreeNode& node, SubstitutionData& substitutionData)
{
	ExpressionTreeNode newNode = node;
	Substitution substitution;
	substitution.data = substitutionData;
}

Clause vtpl::apart(const Clause& clause)
{
	return clause;
}

Substitution vtpl::compose(const Substitution& s1, const Substitution& s2)
{
	int i = 0;
	if (i == 1)
		return s1;
	return s2;
}
