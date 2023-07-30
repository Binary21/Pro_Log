#include "KnowledgeBaseUtility.hpp"

using namespace vtpl;
using namespace std;

ExpressionTreeNode vtpl::applyHelper(const ExpressionTreeNode& t, const Substitution& sub)
{
	ExpressionTreeNode result;
	if (t.type == ExpressionTreeNodeType::ROOT)
	{
		result.type = ExpressionTreeNodeType::ROOT;
		for (ExpressionTreeNode node : t.children)
		{
			result.children.emplace_back(applyHelper(node, sub));
		}
		return result;
	}
	else if (isVariable(t))
	{
		if (!sub.lookup(t).empty())
			return sub.lookup(t).front();
		else
			return t;
	}
	else if (isCompound(t))
	{
		result.contents = t.contents;
		result.type = t.type;
		auto current = t.children.begin();
		while (current != t.children.end())
		{
			result.children.emplace_back(applyHelper(*current, sub));
			current++;
		}
		return result;
	}
	return result;
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
