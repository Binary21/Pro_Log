//=== ExpressionTreeTests.cpp - Catch tests of tokenize implementation
// 
//=================//
#include "catch.hpp"

#include "ExpressionTree.hpp"
#include <sstream>
#include <string>

using namespace vtpl;
using namespace std;


TEST_CASE("")
{

	SECTION("Empty Node")
	{
		ExpressionTreeNode atom;
		REQUIRE(isNone(atom));
	}
	SECTION("Invalid atom and node")
	{
		ExpressionTreeNode atom = makeAtom("2");
		REQUIRE(isNone(atom));
	}
	SECTION("Single Node")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom1 = makeAtom("f");
		REQUIRE(atom1.toStringHelper() == "f");
		REQUIRE(atom1.type == ExpressionTreeNodeType::ATOM);
	}
	SECTION("Two Nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("a");
		std::list<ExpressionTreeNode> arguments = { atom2 };
		ExpressionTreeNode atom1 = makeCompound("f", arguments);
		tree.setRootNode(atom1);
		REQUIRE(atom1.toStringHelper() == "f(a)");
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("a");
		ExpressionTreeNode atom3 = makeAtom("b");
		std::list<ExpressionTreeNode> arguments = { atom2, atom3 };
		ExpressionTreeNode atom1 = makeCompound("f", arguments);
		tree.setRootNode(atom1);
		REQUIRE(atom1.toStringHelper() == "f(a,b)");
		REQUIRE(atom1.type == ExpressionTreeNodeType::COMPOUND);
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atomA = makeAtom("a");
		ExpressionTreeNode atomB = makeAtom("b");
		ExpressionTreeNode atomC = makeAtom("c");
		std::list<ExpressionTreeNode> arguments = { atomB, atomC };
		ExpressionTreeNode atomH = makeCompound("h", arguments);
		arguments = { atomA };
		ExpressionTreeNode atomG = makeCompound("g", arguments);
		arguments = { atomG, atomH };
		ExpressionTreeNode atomF = makeCompound("f", arguments);
		tree.setRootNode(atomF);
		REQUIRE(atomF.toStringHelper() == "f(g(a),h(b,c))");
		REQUIRE(atomF.type == ExpressionTreeNodeType::COMPOUND);
		REQUIRE(isCompound(atomF));
	}

	SECTION("Equal operator")
	{
		ExpressionTreeNode atom1A = makeAtom("a");
		ExpressionTreeNode atom2A = makeAtom("a");
		REQUIRE(atom1A == atom2A);
		ExpressionTreeNodeHasher hasher;
		REQUIRE(hasher(atom1A) == hasher(atom2A));
	}

	ExpressionTreeNode atom = makeAtom("a");
	REQUIRE(atom.toStringHelper() == "a");

	ExpressionTreeNode variable = makeVariable("X");
	REQUIRE(variable.toStringHelper() == "X");

	ExpressionTreeNode root;
	root.type = ExpressionTreeNodeType::ROOT;
	REQUIRE(isNone(root) == false);

}

TEST_CASE("Manual Tests Expression Tree")
{
	ExpressionTreeNode root;
	SECTION("makeAtom")
	{
		root = makeAtom("a&^*");
		REQUIRE(isNone(root));
	}

	SECTION("makeVariable 1")
	{
		root = makeVariable("");
		REQUIRE(isNone(root));
	}

	SECTION("makeVariable 2")
	{
		root = makeVariable("A$%#@");
		REQUIRE(isNone(root));
	}

	SECTION("makeCompound 1")
	{
		list<ExpressionTreeNode> arguments;
		root = makeCompound("", arguments);
		REQUIRE(isNone(root));
		REQUIRE(!isCompound(root));
		root.toString();
	}

	SECTION("makeCompound 2")
	{
		list<ExpressionTreeNode> arguments;
		ExpressionTreeNode node;
		arguments = { node };
		root = makeCompound("a$%#@", arguments);
		REQUIRE(isNone(root));
	}

	SECTION("To String")
	{
		ExpressionTreeNode node;
		node.type = ExpressionTreeNodeType::ROOT;
		ExpressionTreeNode child1 = makeAtom("a");
		ExpressionTreeNode child2 = makeAtom("b");
		node.children = { child1, child2 };
		node.toString();

	}

	SECTION("operator==")
	{
		ExpressionTreeNode nodeAtom = makeAtom("a");
		ExpressionTreeNode nodeVariable = makeVariable("X");
		ExpressionTreeNode nodeCompound = makeCompound("f", { nodeAtom, nodeVariable });
		ExpressionTreeNode nodeCompound2 = makeCompound("g", { nodeAtom, nodeVariable });
		ExpressionTreeNode nodeCompound3 = makeCompound("f", { nodeAtom });
		REQUIRE(!(nodeAtom == nodeVariable));
		REQUIRE(!(nodeCompound == nodeCompound2));
		REQUIRE(!(nodeCompound == nodeCompound3));
		arity(nodeAtom);
	}
}