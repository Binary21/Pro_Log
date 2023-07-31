#ifndef KNOWLEDGEBASEUTILITY_HPP
#define KNOWLEDGEBASEUTILITY_HPP
#include "Unification.hpp"
#include "KnowledgeBase.hpp"

using namespace std;
namespace vtpl {
	ExpressionTreeNode apply(const ExpressionTreeNode& t, const Substitution& sub);
	ExpressionTreeNode applyHelper(const ExpressionTreeNode& t, const Substitution& sub);
	Clause apart(const Clause& clause);
	Substitution compose(const Substitution& s1, const Substitution& s2);
	void standardizeApart(ExpressionTreeNode& node, SubstitutionData& substitutionData, int& counter);
}
#endif // !KNOWLEDGEBASEUTILITY_HPP
