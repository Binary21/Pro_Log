#include "ExpressionTree.hpp"
#include <map>
#include <string>
#include <iostream>



ExpressionTree::ExpressionTree()
{

}

ExpressionTreeNode ExpressionTree::makeAtom(const string& value)
{
	ExpressionTreeNode atom;
	atom.type = ExpressionTreeNodeType::ATOM;
	atom.contents = value;
	return atom;
}

ExpressionTreeNode ExpressionTree::makeVariable(const string& value)
{
	ExpressionTreeNode variable;
	variable.type = ExpressionTreeNodeType::VARIABLE;
	variable.contents = value;
	return variable;
}

ExpressionTreeNode ExpressionTree::makeCompound(const string& predicate, const list<ExpressionTreeNode>& arguments)
{
	ExpressionTreeNode compound;
	compound.type = ExpressionTreeNodeType::COMPOUND;
	compound.contents = predicate;
	compound.children = arguments;
	return compound;
}

bool ExpressionTree::isNone(const ExpressionTreeNode& node) 
{
	if (node.type == ExpressionTreeNodeType::NONE)
		return true;
	return false;
}

bool ExpressionTree::isAtom(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::ATOM)
		return true;
	return false;
}

bool ExpressionTree::isVariable(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::VARIABLE)
		return true;
	return false;
}

bool ExpressionTree::isCompound(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::COMPOUND)
		return true;
	return false;
}

size_t ExpressionTree::arity(const ExpressionTreeNode& node)
{
	size_t childLength = node.children.size();
	return childLength;
}

string ExpressionTree::toString() 
{
	string output = toStringHelper(rootNode);
	cout << output << endl;
	return output;
}


string ExpressionTree::toStringHelper(const ExpressionTreeNode& node)
{
	string result;
	if (isNone(node))
	{
		result += "NONE";
	}
	else if (isAtom(node))
	{
		result += node.contents;
	}
	else if (isCompound(node))
	{
		result += node.contents + "(";
		bool firstArgument = true;
		for (const ExpressionTreeNode& child : node.children)
		{
			if (!firstArgument)
			{
				result += ", ";
			}
			result += toStringHelper(child);
			firstArgument = false;
		}
		result += ")";
	}
	return result;
}

void ExpressionTree::setRootNode(const ExpressionTreeNode& node)
{
	rootNode = node;
}