#include "Lexer.hpp"
#include "Token.hpp"
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
	atom.contents = value;

	if (atom.contents.empty() || isupper(atom.contents[0]) || !isalpha(atom.contents[0]))
	{
		atom.type = ExpressionTreeNodeType::NONE;
	}
	else
	{
		atom.type = ExpressionTreeNodeType::ATOM;
	}

	return atom;
}

ExpressionTreeNode vtpl::makeVariable(const string& value)
{
	ExpressionTreeNode variable;

	variable.contents = value;

	if (variable.contents.empty() || islower(variable.contents[0]) || !isalpha(variable.contents[0]))
	{
		variable.type = ExpressionTreeNodeType::NONE;
	}
	else
	{
		variable.type = ExpressionTreeNodeType::VARIABLE;
	}

	return variable;
}

ExpressionTreeNode vtpl::makeCompound(const string& predicate, const list<ExpressionTreeNode>& arguments)
{
	ExpressionTreeNode compound;

	compound.contents = predicate;
	compound.children = arguments;

	if (compound.contents.empty() || isupper(compound.contents[0]) || arguments.size() == 0 || !isalpha(compound.contents[0]))
	{
		compound.type = ExpressionTreeNodeType::NONE;
	}
	else
	{
		compound.type = ExpressionTreeNodeType::COMPOUND;
	}

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

string ExpressionTreeNode::toString()
{
	return toStringHelper();
}

string ExpressionTreeNode::toStringHelper()
{
	ExpressionTreeNode node = *this;
	string result;

	if (isNone(node))
	{
		result += "NONE";
	}
	else if (isAtom(node))
	{
		result += node.contents;
	}
	else if (isVariable(node))
	{
		result += node.contents;
	}
	else if (isCompound(node))
	{
		result += node.contents + "(";
		bool firstArgument = true;
		for (ExpressionTreeNode& child : node.children)
		{
			if (!firstArgument)
			{
				result += ",";
			}
			result += child.toStringHelper();
			firstArgument = false;
		}
		result += ")";
	}
	else
	{
		bool firstArgument = true;
		result += "(";
		for (ExpressionTreeNode& child : node.children)
		{
			if (!firstArgument)
			{
				result += ",";
			}
			result += child.toStringHelper();
			firstArgument = false;
		}
		result += ")";
	}
	return result;
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