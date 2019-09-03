#ifndef __COMPILER_SYNTAX_ANALYZE_H
#define __COMPILER_SYNTAX_ANALYZE_H

#include <memory>
#include <list>
#include <vector>

#include "common.h"
#include "utils/token_handler.h"

class SimpleASTNode;
enum class ASTNodeType
{
    INT_DECLARATION,
    INT_LITERAL,
    ID,
    ASSIGMENT,
    ADDITIVE_EXP,
    MULTIL_EXP,
};

class SimpleASTNode
{
public:
    using Ptr = std::shared_ptr<SimpleASTNode>;
    SimpleASTNode() {}
    SimpleASTNode(const ASTNodeType& type, const std::string& text)
        :m_type(type), m_text(text) {}
    static Ptr create(const ASTNodeType& type, const std::string& text);
    void addChild(Ptr node);
    void print(int level);
private:
    ASTNodeType m_type;
    std::string m_text;
    std::list<Ptr> m_childList;
};

class SimpleSyntaxAnalyzer : public Singleton<SimpleSyntaxAnalyzer>
{
public:
    ~SimpleSyntaxAnalyzer() {};
    SimpleASTNode::Ptr intDelare(TokenReader& tokens);
private:
    SimpleSyntaxAnalyzer() {};
    friend Singleton<SimpleSyntaxAnalyzer>;
};

#endif