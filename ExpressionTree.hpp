#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP
#include "Token.hpp"
#include <cstddef>
#include <list>
#include <ostream>
#include <string>

using namespace vtpl;
using namespace std;

enum class ExpressionTreeNodeType {
	NONE,
	ATOM,
	VARIABLE,
	COMPOUND
};

struct ExpressionTreeNode {
	ExpressionTreeNodeType type;
	string contents;
	list<ExpressionTreeNode> children;

	ExpressionTreeNode() : type(ExpressionTreeNodeType::NONE), contents("") {};
	ExpressionTreeNode(ExpressionTreeNodeType t, const string& c) : type(t), contents(c) {};
};

class ExpressionTree {
public:
	ExpressionTree();
	std::string toString();
	
	ExpressionTreeNode makeAtom(const string& value);
	ExpressionTreeNode makeVariable(const string& value);
	ExpressionTreeNode makeCompound(const string& predicate, const list<ExpressionTreeNode>& arguments);

	bool isNone(const ExpressionTreeNode& node);
	bool isAtom(const ExpressionTreeNode& node);
	bool isVariable(const ExpressionTreeNode& node);
	bool isCompound(const ExpressionTreeNode& node);

	static size_t arity(const ExpressionTreeNode& node);
	void setRootNode(const ExpressionTreeNode& node);


private:
	string toStringHelper(const ExpressionTreeNode& node);
	
	ExpressionTreeNode rootNode;
};

#endif
