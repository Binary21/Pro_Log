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
	if (x.type == ExpressionTreeNodeType::ROOT && y.type == ExpressionTreeNodeType::ROOT)
	{
		auto current_x = X.children.begin();
		auto current_y = Y.children.begin();

		while (current_x != X.children.end())
		{
			unify(*current_x, *current_y, subst);
			++current_x;
			++current_y;
		}
		return;

	}

	if (subst.failed == true)
		return;
	else if (X == Y)
		return;
	// else if isvariable(x) then return unify-var(x,y,s)
	else if (isVariable(X))
		unifyVar(X, Y, subst);
	// else if isvariable(y) then return unify-var(y,x,s)
	else if (isVariable(Y))
		unifyVar(Y, X, subst);

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
			unify(*current_x, *current_y, subst);
			++current_x;
			++current_y;
		}
		return;
	}
	else
		subst.failed = true;
}

void vtpl::unifyVar(const ExpressionTreeNode& var, const ExpressionTreeNode& x, UnificationResult& subst)
{
	auto lookup_var = subst.substitution.lookup(var);
	if (!lookup_var.empty())
	{
		unify(lookup_var.front(), x, subst);
	}
	else
	{
		auto lookup_x = subst.substitution.lookup(x);
		if (!lookup_x.empty())
		{
			unify(var, lookup_x.front(), subst);
		}
		else
			subst.substitution.insert(var, x);
	}
}