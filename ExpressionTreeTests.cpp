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
		ExpressionTreeNode atom1 = makeAtom("F");
		REQUIRE(tree.toString() == "NONE");
		REQUIRE(atom1.type == ExpressionTreeNodeType::ATOM);
	}
	SECTION("Two Nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("A");
		std::list<ExpressionTreeNode> arguments = { atom2 };
		ExpressionTreeNode atom1 = makeCompound("F", arguments);
		tree.setRootNode(atom1);
		REQUIRE(tree.toString() == "F(A)");
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("A");
		ExpressionTreeNode atom3 = makeAtom("B");
		std::list<ExpressionTreeNode> arguments = { atom2, atom3 };
		ExpressionTreeNode atom1 = makeCompound("F", arguments);
		tree.setRootNode(atom1);
		REQUIRE(tree.toString() == "F(A, B)");
		REQUIRE(atom1.type == ExpressionTreeNodeType::COMPOUND);
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atomA = makeAtom("A");
		ExpressionTreeNode atomB = makeAtom("B");
		ExpressionTreeNode atomC = makeAtom("C");
		std::list<ExpressionTreeNode> arguments = { atomB, atomC };
		ExpressionTreeNode atomH = makeCompound("H", arguments);
		arguments = { atomA };
		ExpressionTreeNode atomG = makeCompound("G", arguments);
		arguments = { atomG, atomH };
		ExpressionTreeNode atomF = makeCompound("F", arguments);
		tree.setRootNode(atomF);
		REQUIRE(tree.toString() == "F(G(A), H(B, C))");
		REQUIRE(atomF.type == ExpressionTreeNodeType::COMPOUND);
		REQUIRE(isCompound(atomF));
	}
	SECTION("Emplace Test")
	{
		ExpressionTree tree;
		ExpressionTreeNode atomA = makeAtom("A");
		ExpressionTreeNode atomB = makeAtom("B");
		ExpressionTreeNode atomC = makeAtom("C");
		atomA.children.emplace_back(atomB);
		atomA.children.emplace_back(atomC);
		tree.setRootNode(atomA);
		tree.toString();
	}

	SECTION("Equal operator")
	{
		ExpressionTree tree1, tree2;
		ExpressionTreeNode atom1A= makeAtom("A");
		ExpressionTreeNode atom1B = makeAtom("B");
		ExpressionTreeNode atom2A = makeAtom("A");
		ExpressionTreeNode atom2B = makeAtom("B");
		list<ExpressionTreeNode> argument1 = { atom1A, atom1B };
		list<ExpressionTreeNode> argument2 = { atom2A, atom2B };

		ExpressionTreeNode atom1C = makeCompound("F", argument1);
		ExpressionTreeNode atom2C = makeCompound("F", argument2);
		tree1.setRootNode(atom1C);
		tree2.setRootNode(atom2C);
		REQUIRE(tree1 == tree2);
	}

}