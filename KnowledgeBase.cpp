#include <string>
#include <iostream>
#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"
#include "catch.hpp"

using namespace std;

void vtpl::KnowledgeBase::tell(const Clause& clause)
{
	clauses.push_back(clause);
}

list<vtpl::Substitution> vtpl::KnowledgeBase::ask(const ExpressionTreeNode& query) const
{
	ExpressionTreeNode input;
	if (query.type == ExpressionTreeNodeType::ROOT)
		input = query.children.front();
	else
		input = query;

	list<ExpressionTreeNode> goals = { input };
	Substitution result;
	list<Substitution> lists = folbc(goals, result);
	return lists;
}

list<vtpl::Substitution> vtpl::KnowledgeBase::folbc(list<ExpressionTreeNode>& goals, Substitution& s) const
{
	if (goals.empty())
		return { s }; //  just return s
	

	list<Substitution> answers;
	ExpressionTreeNode q1 = apply(goals.front(), s);
	Iterator it = begin();
	while (it != end())
	{
		Clause apartClause = apart(*it);
		UnificationResult result;
		result.substitution = s;

		unify(apartClause.head, q1, result);
		if (result.failed == false)
		{
			list<ExpressionTreeNode> newGoals;

			if (apartClause.body.type == ExpressionTreeNodeType::ROOT)
			{
				for (ExpressionTreeNode children : apartClause.body.children)
				{
					cout << "body: " << children.toString() << endl;
					newGoals.emplace_back(children);
				}
			}
			int its = 1;
			for (ExpressionTreeNode nodes : goals)
			{
				if (its != 1)
				{
					newGoals.emplace_back(nodes);
				}
				its++;
			}

			Substitution composedSub = compose(s, result.substitution);
			list<Substitution> folbcResult = folbc(newGoals, composedSub);
			list<Substitution> unioning = unionize(folbcResult, answers);

			for (Substitution subst : unioning)
			{
				answers.emplace_back(subst);
			}

			
		}
		it++;
	}
	return answers;
}

list<vtpl::Substitution> vtpl::unionize(list<Substitution> s2, list<Substitution> s)
{
	list<Substitution> result;
	result = s;
	for (Substitution subst2 : s2)
	{
		Substitution newSubstitution;
		for (auto pair2 : subst2.data) // assuming data is a list of pairs
		{
			bool found = false;

			for (Substitution subst1 : result)
			{
				for (auto pair1 : subst1.data) // assuming data is a list of pairs
				{
					if (pair1 == pair2)
					{
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
			if (!found)
				newSubstitution.data.insert(pair2);
		}
		if(!newSubstitution.data.empty())
			result.emplace_back(newSubstitution);
	}
	return result;
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
	return clauses.end();
}