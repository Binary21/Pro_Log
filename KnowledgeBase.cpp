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
	return folbc(goals, result);
}

list<vtpl::Substitution> vtpl::KnowledgeBase::folbc(list<ExpressionTreeNode>& goals, Substitution& s) const
{
	if (goals.empty())
		return { s }; //  just return s


	list<Substitution> answers;
	//cout << "pre application " << goals.front().toString() << endl;
	ExpressionTreeNode q1 = apply(goals.front(), s);
	//cout << "application: " << q1.toString() << endl;
	Iterator it = begin();
	while (it != end())
	{
		Clause apartClause = apart(*it);
		UnificationResult result;
		unify(apartClause.head, q1, result);
		//string pass = "SUCCEEDED";
		//if (result.failed == false)
			//cout << "Atempting to unify " << apartClause.head.toString() << " and " << q1.toString() << " " << pass << endl;
		if (result.failed == false)
		{
			list<ExpressionTreeNode> newGoals = goals;
			newGoals.pop_front();

			if (apartClause.body.type == ExpressionTreeNodeType::ROOT)
			{
				for (ExpressionTreeNode children : apartClause.body.children)
				{
					//cout << "newGoal: " << children.toString() << endl;
					newGoals.push_front(children);
				}
			}
			Substitution composedSub = compose(result.substitution, s);
			answers = unionize(folbc(newGoals, composedSub), answers);
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
					//cout << "comparing: {" << pair1.first.toString() << "/" << pair1.second.toString() << "}" << " and {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
					if (pair1 == pair2)
					{
						//cout << "successful: {" << pair1.first.toString() << "/" << pair1.second.toString() << "}" << " and {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
			if (!found)
			{
				//cout << "inserting: {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
				newSubstitution.data.insert(pair2);
			}
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