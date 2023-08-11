#include "KnowledgeBaseUtility.hpp"
#include <atomic>

using namespace vtpl;
using namespace std;


ExpressionTreeNode vtpl::applyHelper(const ExpressionTreeNode& t, const Substitution& sub)
{
	ExpressionTreeNode result;
	if (t.type == ExpressionTreeNodeType::ROOT || (t.contents == "" && t.children.size() > 0))
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
		auto subResult = sub.lookup(t);

		if (!subResult.empty())
		{
			return subResult.front();
		}
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
	else if (isAtom(t))
		result = t;
	return result;
}

ExpressionTreeNode vtpl::apply(const ExpressionTreeNode& t, const Substitution& sub)
{
	return (applyHelper(t, sub));
}

void vtpl::standardizeApart(ExpressionTreeNode& node, SubstitutionData& substitutionData, std::atomic<int>& counter, bool isBody)
{
	if (isVariable(node))
	{
		auto it = substitutionData.find(node);
		if (it == substitutionData.end() && isBody == false)
		{
			counter++;
			string newNameStr = node.contents + "_" + to_string(counter);
			ExpressionTreeNode newName;
			newName.type = ExpressionTreeNodeType::VARIABLE;
			newName.contents = newNameStr;
			substitutionData.insert({ node, newName });
			node.contents = newNameStr;
			
		}
		else if (it != substitutionData.end())
		{
			node.contents = it->second.contents;
		}
	}
	for (auto& child : node.children)
	{
		standardizeApart(child, substitutionData, counter, isBody);
	}
}

Clause vtpl::apart(const Clause& clause)
{
	std::atomic<int> counter(0);
	bool isBody = false;
	Clause newClause = clause;
	SubstitutionData substData;

	standardizeApart(newClause.head, substData, counter, isBody);
	isBody = true;
	standardizeApart(newClause.body, substData, counter, isBody);
	return newClause;
}

Substitution vtpl::compose(const Substitution& s1, const Substitution& s2)
{
	Substitution result;
	for (auto it = s1.constBegin(); it != s1.constEnd(); it++)
	{
		ExpressionTreeNode k = it->first;
		ExpressionTreeNode v1 = it->second;


		list<ExpressionTreeNode> v2 = s2.lookup(v1);
		if (v2.size() == 0)
		{
			result.insert(k, v1);
		}
		else
		{
			for (ExpressionTreeNode v2node : v2)
			{
				result.insert(k, apply(v2node, s1));
			}
		}
	}

	for (auto it = s2.constBegin(); it != s2.constEnd(); it++)
	{
		ExpressionTreeNode k = it->first;
		if (result.lookup(k).size() == 0)
		{
			ExpressionTreeNode k = it->first;
			ExpressionTreeNode v2 = it->second;
			result.insert(k, v2);
		}
	}
	return result;
}
