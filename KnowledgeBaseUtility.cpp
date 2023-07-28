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


void vtpl::standardizeApart(ExpressionTreeNode& node, std::unordered_map<std::string, std::string>& dict)
{
	if (isVariable(node))
	{
		auto it = dict.find(node.contents);
		if (it == dict.end())
		{
			counterDict[node.contents]++;
			std::string newName = node.contents + "_" + std::to_string(counterDict[node.contents]);
			dict[node.contents] = newName;
			node.contents = newName;
		}
		else
			node.contents = it->second;
	}

	for (auto& child : node.children)
	{
		standardizeApart(child, dict);
	}
}

Clause vtpl::apart(const Clause& clause)
{
	Clause newClause = clause;
	std::unordered_map<std::string, std::string> dict;

	standardizeApart(newClause.head, dict);
	standardizeApart(newClause.body, dict);

	return newClause;
}

Substitution vtpl::compose(const Substitution& s1, const Substitution& s2)
{
	return Substitution();
}
