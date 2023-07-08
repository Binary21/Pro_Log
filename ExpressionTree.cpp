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
	return false;;
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

bool vtpl::ExpressionTree::operator==(const ExpressionTree rhs) const
{
	return this->toString() == rhs.toString();
}



std::string ExpressionTree::toString() const
{
	std::string output = toStringHelper(rootNode);
	return output;
}

std::string vtpl::ExpressionTree::toStringHelper(const ExpressionTreeNode& node) const
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
std::string vtpl::treeToString(const ExpressionTreeNode& node)
{
	std::string result;
	switch (node.type) {
	case vtpl::ExpressionTreeNodeType::NONE:
		result += "NONE";
		break;
	case vtpl::ExpressionTreeNodeType::ATOM:
		result += "ATOM " + node.contents;
		break;
	case vtpl::ExpressionTreeNodeType::VARIABLE:
		result += "VARIABLE " + node.contents;
		break;
	case vtpl::ExpressionTreeNodeType::COMPOUND:
		result += "COMPOUND " + node.contents + " [";
		for (const auto& child : node.children) {
			result += treeToString(child) + ", ";
		}
		result += "]";
		break;
	}
	return result;
}
void vtpl::ExpressionTree::setRootNode(const ExpressionTreeNode& node)
{
	rootNode = node;
}

std::size_t vtpl::ExpressionTreeNodeHasher::operator()(const vtpl::ExpressionTreeNode& node) const
{
	std::string node_str = treeToString(node);
	std::hash<std::string> str_hash;
	return str_hash(node_str);
}