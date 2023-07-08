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
	SECTION("Single Node")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom1 = tree.makeAtom("F");
		REQUIRE(tree.toString() == "NONE");
		REQUIRE(atom1.type == ExpressionTreeNodeType::ATOM);
	}
	SECTION("Two Nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = tree.makeAtom("A");
		std::list<ExpressionTreeNode> arguments = { atom2 };
		ExpressionTreeNode atom1 = tree.makeCompound("F", arguments);
		tree.setRootNode(atom1);
		REQUIRE(tree.toString() == "F(A)");
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = tree.makeAtom("A");
		ExpressionTreeNode atom3 = tree.makeAtom("B");
		std::list<ExpressionTreeNode> arguments = { atom2, atom3 };
		ExpressionTreeNode atom1 = tree.makeCompound("F", arguments);
		tree.setRootNode(atom1);
		REQUIRE(tree.toString() == "F(A, B)");
		REQUIRE(atom1.type == ExpressionTreeNodeType::COMPOUND);
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atomA = tree.makeAtom("A");
		ExpressionTreeNode atomB = tree.makeAtom("B");
		ExpressionTreeNode atomC = tree.makeAtom("C");
		std::list<ExpressionTreeNode> arguments = { atomB, atomC };
		ExpressionTreeNode atomH = tree.makeCompound("H", arguments);
		arguments = { atomA };
		ExpressionTreeNode atomG = tree.makeCompound("G", arguments);
		arguments = { atomG, atomH };
		ExpressionTreeNode atomF = tree.makeCompound("F", arguments);
		tree.setRootNode(atomF);
		REQUIRE(tree.toString() == "F(G(A), H(B, C))");
		REQUIRE(atomF.type == ExpressionTreeNodeType::COMPOUND);
		REQUIRE(isCompound(atomF));
	}
	SECTION("Emplace Test")
	{
		ExpressionTree tree;
		ExpressionTreeNode atomA = tree.makeAtom("A");
		ExpressionTreeNode atomB = tree.makeAtom("B");
		ExpressionTreeNode atomC = tree.makeAtom("C");
		atomA.children.emplace_back(atomB);
		atomA.children.emplace_back(atomC);
		tree.setRootNode(atomA);
		tree.toString();
	}

}