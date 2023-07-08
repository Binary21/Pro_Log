#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include <cstddef>
#include <list>
#include <iostream>
#include <string>
#include <functional>

namespace vtpl {

    enum class ExpressionTreeNodeType {
        NONE,
        ATOM,
        VARIABLE,
        COMPOUND
    };

    

    struct ExpressionTreeNode {
        ExpressionTreeNodeType type;
        std::string contents;
        std::list<ExpressionTreeNode> children;
        std::string toString();
        

        ExpressionTreeNode() : type(ExpressionTreeNodeType::NONE), contents("") {}
        ExpressionTreeNode(ExpressionTreeNodeType t, const std::string& c) : type(t), contents(c) {}

        bool operator==(const ExpressionTreeNode rhs) const;
    };

    struct ExpressionTreeNodeHasher
    {
        std::size_t operator()(const ExpressionTreeNode& node) const;
    };

    class ExpressionTree {
    public:
        ExpressionTree();
        
        void setRootNode(const ExpressionTreeNode& node);

    private:

        ExpressionTreeNode rootNode;
       
    };
    std::string treeToString(const ExpressionTreeNode& node);
    bool isNone(const ExpressionTreeNode& node);
    bool isAtom(const ExpressionTreeNode& node);
    bool isVariable(const ExpressionTreeNode& node);
    bool isCompound(const ExpressionTreeNode& node);

    ExpressionTreeNode makeAtom(const std::string& value);
    ExpressionTreeNode makeVariable(const std::string& value);
    ExpressionTreeNode makeCompound(const std::string& predicate, const std::list<ExpressionTreeNode>& arguments);

    size_t arity(const ExpressionTreeNode& node);

} // namespace vtpl

#endif