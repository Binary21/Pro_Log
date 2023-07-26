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
	ExpressionTreeNode X = x;
	ExpressionTreeNode Y = y;
	if (x.type == ExpressionTreeNodeType::ROOT)
	{
		X = x.children.front();
	}
	if (y.type == ExpressionTreeNodeType::ROOT)
	{
		Y = y.children.front();
	}

	cout << "X: " << X.toString() << endl;
	cout << "Y: " << Y.toString() << endl;
	if (subst.failed == true)
		return;
	else if (X == Y)
		return;
	// else if isvariable(x) then return unify-var(x,y,s)
	else if (isVariable(X))
	{
		if (!subst.substitution.lookup(X).empty())
			subst.substitution.insert(X, Y);
		else
			subst.substitution.insert(X, Y);
	}
	// else if isvariable(y) then return unify-var(y,x,s)
	else if (isVariable(Y))
		unify(Y, X, subst);

	// else if iscompound(x) and ifcompound(y) then return unify(x.args, y.args, unify(x.op, y.op, s))
	else if (isCompound(X) && isCompound(Y))
	{
		if (X.contents != Y.contents || X.children.size() != Y.children.size())
		{
			subst.failed = true;
			return;
		}
		auto current_x = X.children.begin();
		auto current_y = Y.children.begin();

		while (current_x != X.children.end())
		{
			cout << "current X: " << current_x->toString() << endl;
			cout << "current X: " << current_y->toString() << endl;
			unify(*current_x, *current_y, subst);
			++current_x;
			++current_y;
		}
		return;
	}
	else
	{
		subst.failed = true;
		return;
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