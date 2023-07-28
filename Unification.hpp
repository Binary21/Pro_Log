#ifndef UNIFICATION_HPP
#define UNIFICATION_HPP

#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include "ExpressionTree.hpp"

using namespace std;
namespace vtpl {
	
	typedef std::unordered_multimap<ExpressionTreeNode, ExpressionTreeNode, ExpressionTreeNodeHasher> SubstitutionData;

	class Substitution {
	public:
		typedef typename SubstitutionData::iterator IteratorType;
		typedef typename SubstitutionData::const_iterator ConstIteratorType;

		std::list<ExpressionTreeNode> lookup(const ExpressionTreeNode& key) const;

		void insert(const ExpressionTreeNode& key, const ExpressionTreeNode& value);

		IteratorType begin();
		IteratorType end();

		ConstIteratorType constBegin() const;
		ConstIteratorType constEnd() const;

		SubstitutionData data;

	private:
		
	};

	struct UnificationResult
	{
		bool failed = false;
		Substitution substitution;
	};

	void unify(const ExpressionTreeNode& x, const ExpressionTreeNode& y, UnificationResult& subst);
	void unifyVar(const ExpressionTreeNode& var, const ExpressionTreeNode& x, UnificationResult& subst);
}
#endif