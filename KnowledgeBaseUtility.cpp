#include "KnowledgeBaseUtility.hpp"

using namespace vtpl;
using namespace std;

ExpressionTreeNode vtpl::apply(const ExpressionTreeNode& t, const Substitution& sub)
{
	ExpressionTreeNode T = t;
	if (t.type == ExpressionTreeNodeType::ROOT)
	{
		T = t.children.front();
	}
	ExpressionTreeNode application;
	if (isVariable(T))
	{
		if (!sub.lookup(T).empty())
		{
			application = sub.lookup(T).front();
			return application;
		}
	}
	else if (isCompound(T))
	{
		application.type = T.type;
		application.contents = T.contents;
		auto current = T.children.begin();
		while (current != T.children.end())
		{
			application.children.push_back(apply(*current, sub));
			current++;
		}
		return application;
	}
	return t;
}

void vtpl::standardizeApart(ExpressionTreeNode& node, SubstitutionData& substitutionData)
{
	if (isVariable(node))
	{
		auto it = substitutionData.find(node);
		if (it == substitutionData.end())
		{
			counterDict[node.contents]++;
			std::string newNameStr = node.contents + "_" + std::to_string(counterDict[node.contents]);
			ExpressionTreeNode newName;
			newName.type = ExpressionTreeNodeType::VARIABLE;
			newName.contents = newNameStr;
			substitutionData.insert({ node, newName });
			node.contents = newNameStr;
		}
		else
			node.contents = it->second.contents;
	}

	for (auto& child : node.children)
	{
		standardizeApart(child, substitutionData);
	}
}

Clause vtpl::apart(const Clause& clause)
{
	Clause newClause = clause;
	SubstitutionData substitutionData;

	standardizeApart(newClause.head, substitutionData);
	standardizeApart(newClause.body, substitutionData);

	return newClause;
}

Substitution vtpl::compose(const Substitution& s1, const Substitution& s2)
{
	Substitution result;
	for (auto it = s1.constBegin(); it != s1.constEnd(); it++)
	{
		ExpressionTreeNode k = it->first;
		cout << k.toString() << endl;
		ExpressionTreeNode v1 = it->second;


		list<ExpressionTreeNode> v2 = s2.lookup(k);
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
		if(result.lookup(k).size() == 0)
		{
			ExpressionTreeNode k = it->first;
			ExpressionTreeNode v2 = it->second;
			result.insert(k, v2);
		}
	}
	return result;
}
