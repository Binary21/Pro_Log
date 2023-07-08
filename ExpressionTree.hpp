#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include <cstddef>
#include <list>
#include <ostream>
#include <string>

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

        ExpressionTreeNode() : type(ExpressionTreeNodeType::NONE), contents("") {}
        ExpressionTreeNode(ExpressionTreeNodeType t, const std::string& c) : type(t), contents(c) {}
    };

    class ExpressionTree {
    public:
        ExpressionTree();
        std::string toString();

        ExpressionTreeNode makeAtom(const std::string& value);
        ExpressionTreeNode makeVariable(const std::string& value);
        ExpressionTreeNode makeCompound(const std::string& predicate, const std::list<ExpressionTreeNode>& arguments);

        static size_t arity(const ExpressionTreeNode& node);
        void setRootNode(const ExpressionTreeNode& node);

    private:
        std::string toStringHelper(const ExpressionTreeNode& node);

        ExpressionTreeNode rootNode;
    };

    bool isNone(const ExpressionTreeNode& node);
    bool isAtom(const ExpressionTreeNode& node);
    bool isVariable(const ExpressionTreeNode& node);
    bool isCompound(const ExpressionTreeNode& node);

} // namespace vtpl

#endif