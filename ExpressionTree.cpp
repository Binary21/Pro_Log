#include "ExpressionTree.hpp"
#include <map>
#include <string>
#include <iostream>

using namespace vtpl;
using namespace std;

ExpressionTree::ExpressionTree()
{

}

ExpressionTreeNode vtpl::makeAtom(const string& value)
{
	ExpressionTreeNode atom;
	atom.type = ExpressionTreeNodeType::ATOM;
	atom.contents = value;

	if (!atom.children.empty())
	{
		atom.type = ExpressionTreeNodeType::COMPOUND;
	}

	if (atom.contents.empty())
	{
		atom.type = ExpressionTreeNodeType::NONE;
	}

	return atom;
}

ExpressionTreeNode vtpl::makeVariable(const string& value)
{
	ExpressionTreeNode variable;
	variable.type = ExpressionTreeNodeType::VARIABLE;
	variable.contents = value;
	return variable;
}

ExpressionTreeNode vtpl::makeCompound(const string& predicate, const list<ExpressionTreeNode>& arguments)
{
	ExpressionTreeNode compound;
	compound.type = ExpressionTreeNodeType::COMPOUND;
	compound.contents = predicate;
	compound.children = arguments;
	return compound;
}

bool vtpl::isNone(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::NONE)
		return true;
	return false;
}

bool vtpl::isAtom(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::ATOM)
		return true;
	return false;
}

bool vtpl::isVariable(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::VARIABLE)
		return true;
	return false;
}

bool vtpl::isCompound(const ExpressionTreeNode& node)
{
	if (node.type == ExpressionTreeNodeType::COMPOUND)
		return true;
	return false;
}

size_t vtpl::arity(const ExpressionTreeNode& node)
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

bool ExpressionTreeNode::operator==(const ExpressionTreeNode rhs) const
{
	if (type != rhs.type)
		return false;
	if (contents != rhs.contents)
		return false;
	if (children != rhs.children)
		return false;
	return true;
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

std::size_t ExpressionTreeNodeHasher::operator()(const ExpressionTreeNode& node) const
{
	std::string tree_string = treeToString(node);
	return std::hash<std::string>{}(tree_string);
}

std::string vtpl::treeToString(const ExpressionTreeNode& node) 
{
	std::string result = node.contents;
	for (const auto& child : node.children) {
		result += "(" + treeToString(child) + ")";
	}
	return result;
}