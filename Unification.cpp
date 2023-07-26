#include "Unification.hpp"
#include <string>
#include <iostream>

using namespace vtpl;
using namespace std;

std::list<ExpressionTreeNode> vtpl::Substitution::lookup(const ExpressionTreeNode& key) const
{
	auto range = data.equal_range(key);
	std::list<ExpressionTreeNode> result;
	for (auto it = range.first; it != range.second; ++it)
	{
		result.push_back(it->second);
	}
	return result;
}

void vtpl::Substitution::insert(const ExpressionTreeNode& key, const ExpressionTreeNode& value)
{
	data.insert({ key, value });
}

Substitution::IteratorType vtpl::Substitution::begin() {
	return data.begin();
}

Substitution::IteratorType vtpl::Substitution::end() {
	return data.end();
}

Substitution::ConstIteratorType vtpl::Substitution::constBegin() const {
	return data.cbegin();
}

Substitution::ConstIteratorType vtpl::Substitution::constEnd() const {
	return data.cend();
}

void vtpl::unify(const ExpressionTreeNode& x, const ExpressionTreeNode& y, UnificationResult& subst)
{
	cout << "X: " << x.toString() << endl;
	cout << "Y: " << y.toString() << endl;

	if (isVariable(x))
	{
		if (!subst.substitution.lookup(x).empty())
			unifyVar(subst.substitution.lookup(x).front(), y, subst);
		else
			subst.substitution.insert(x, y);
	}
	else if (isVariable(y))
		unify(y, x, subst);
	else if (isCompound(x) && isCompound(y))
	{
		if (x.contents != y.contents || x.children.size() != y.children.size())
		{
			subst.failed = true;
			return;
		}
		auto current_x = x.children.begin();
		auto current_y = y.children.begin();

		while (current_x != x.children.end())
		{
			unify(*current_x, *current_y, subst);
			++current_x;
			++current_y;
		}
	}
}

void vtpl::unifyVar(const ExpressionTreeNode& var, const ExpressionTreeNode& x, UnificationResult& subst)
{
	if (!subst.substitution.lookup(var).empty())
	{
		unify(var, x, subst);
	}
	else if (!subst.substitution.lookup(x).empty())
	{
		unify(var, x, subst);
	}
}