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
	list<ExpressionTreeNode> goals = { query };
	Substitution result;
	return folbc(*this, goals, result);
}

list<vtpl::Substitution> vtpl::KnowledgeBase::folbc(KnowledgeBase kb, list<ExpressionTreeNode> goals, Substitution& s) const
{
	if (goals.empty())
		return { s }; //  just return s


	list<Substitution> answers;
	ExpressionTreeNode q1 = apply(goals.front(), s);
	cout << "q1: " << q1.toString() << endl;

	for (Clause r : kb)
	{
		Clause apartClause = apart(r);
		// where s2 = unify(q2, q1, s) succeeds is unification of apartClause.head and q1
		UnificationResult result;
		result.substitution = s;
		unify(apartClause.head, q1, result);
		string pass = "SUCCEEDED";
		if (result.failed == false)
			pass = "FAILED";
		cout << "Attempting to unify " << apartClause.head.toString() << " and " << q1.toString() << pass << endl;
		if (result.failed != false)
		{
			list<ExpressionTreeNode> newGoals;
			newGoals = goals;
			if (apartClause.body.type == ExpressionTreeNodeType::ROOT)
			{
				for (ExpressionTreeNode children : apartClause.body.children)
				{
					newGoals.push_front(children);
				}
			}
			cout << "NEW GOALS:-----------------------------------------------------" << endl;
			for (ExpressionTreeNode node : newGoals)
			{
				cout << node.toString() << endl;
			}
			answers = unionize(folbc(kb, newGoals, compose(result.substitution, s)), answers);
		}
		// new goals is body of q2 plus already existing goals

		// write my own union function
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
					cout << "comparing: {" << pair1.first.toString() << "/" << pair1.second.toString() << "}" << " and {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
					if (pair1 == pair2)
					{
						cout << "successful: {" << pair1.first.toString() << "/" << pair1.second.toString() << "}" << " and {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
			if (!found)
			{
				cout << "inserting: {" << pair2.first.toString() << "/" << pair2.second.toString() << "}" << endl;
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