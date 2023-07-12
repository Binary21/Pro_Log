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



		// member function that does both of these things
		// never actuall call emplace_back
		//atom.children.emplace_back(atom2);
		//atom.type = ExpressionTreeNodeType::COMPOUND;
	}
	SECTION("Single Node")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom1 = makeAtom("f");
		REQUIRE(atom1.toString() == "f");
		REQUIRE(atom1.type == ExpressionTreeNodeType::ATOM);
	}
	SECTION("Two Nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("a");
		std::list<ExpressionTreeNode> arguments = { atom2 };
		ExpressionTreeNode atom1 = makeCompound("f", arguments);
		tree.setRootNode(atom1);
		REQUIRE(atom1.toString() == "f(a)");
	}
	SECTION("1 Parent node 2 Children nodes")
	{
		ExpressionTree tree;
		ExpressionTreeNode atom2 = makeAtom("a");
		ExpressionTreeNode atom3 = makeAtom("b");
		std::list<ExpressionTreeNode> arguments = { atom2, atom3 };
		ExpressionTreeNode atom1 = makeCompound("f", arguments);
		tree.setRootNode(atom1);
		REQUIRE(atom1.toString() == "f(a,b)");
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
		REQUIRE(atomF.toString() == "f(g(a),h(b,c))");
		REQUIRE(atomF.type == ExpressionTreeNodeType::COMPOUND);
		REQUIRE(isCompound(atomF));
	}

	SECTION("Equal operator")
	{
		ExpressionTreeNode atom1A= makeAtom("a");
		ExpressionTreeNode atom2A = makeAtom("a");
		REQUIRE(atom1A == atom2A);
		ExpressionTreeNodeHasher hasher;
		REQUIRE(hasher(atom1A) == hasher(atom2A));
	}

	ExpressionTreeNode atom = makeAtom("a");
	REQUIRE(atom.toString() == "a");


}