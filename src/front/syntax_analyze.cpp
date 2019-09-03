#include <iostream>
#include <string>

#include "front/syntax_analyze.h"
#include "utils/token_handler.h"

static SimpleASTNode::Ptr additive(TokenReader& tokens);
static SimpleASTNode::Ptr multiplicative(TokenReader& tokens);
static SimpleASTNode::Ptr intLiteral(TokenReader& tokens);

static std::string syntaxType(ASTNodeType& type)
{
    switch (type) {
        case ASTNodeType::INT_DECLARATION:
            return "Int Key Word";
        case ASTNodeType::ADDITIVE_EXP:
            return "Additive";
        case ASTNodeType::MULTIL_EXP:
            return "Multiplicative";
        case ASTNodeType::ASSIGMENT:
            return "Assignment";
        case ASTNodeType::ID:
            return "Variable ID";
        case ASTNodeType::INT_LITERAL:
            return "Int Literal";
    }
}

static SimpleASTNode::Ptr intLiteral(TokenReader& tokens)
{
    SimpleASTNode::Ptr node = nullptr;
    Token token = tokens.peek();
    if (!token.isEmpty() && token.type == INT_LITERAL_TYPE) {
        token = tokens.read();
        node = SimpleASTNode::create(ASTNodeType::INT_LITERAL, token.val);
    }
    return node;
}

static SimpleASTNode::Ptr multiplicative(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = intLiteral(tokens);
    SimpleASTNode::Ptr node = child1;
    Token token = tokens.peek();
    if (node != nullptr && !token.isEmpty()) {
        if (token.type == STAR_TYPE) {
            token = tokens.read();
            SimpleASTNode::Ptr child2;
            try {
                child2 = multiplicative(tokens);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::terminate();
            }
            if (child2 != nullptr) {
                node = SimpleASTNode::create(ASTNodeType::MULTIL_EXP, token.val);
                node->addChild(child1);
                node->addChild(child2);
            } else {
                throw std::invalid_argument("invalid multiplicative expression, expect the right part");
            }
        }
    }
    return node;
}

static SimpleASTNode::Ptr additive(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1;
    try {
        child1 = multiplicative(tokens);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::terminate();
    }

    SimpleASTNode::Ptr node = child1;
    Token token = tokens.peek();
    if (node != nullptr && !token.isEmpty()) {
        if (token.type == PLUS_TYPE) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = nullptr;
            try {
                child2 = additive(tokens);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::terminate();
            }
            if (child2 != nullptr) {
                node = SimpleASTNode::create(ASTNodeType::ADDITIVE_EXP, token.val);
                node->addChild(child1);
                node->addChild(child2);
            } else {
                throw std::invalid_argument("invalid additive expression, expect the right part!");
            }
        }
    }
    return node;
}

SimpleASTNode::Ptr SimpleASTNode::create(const ASTNodeType& type, const std::string& text)
{
    return std::make_shared<SimpleASTNode>(type, text);
}

void SimpleASTNode::addChild(SimpleASTNode::Ptr node)
{
    m_childList.push_back(node);
}

void SimpleASTNode::print(int level)
{
    if (!level)
        std::cout << std::endl;
    for (int i = 0; i < level; i++)
        std::cout << '\t';
    std::cout << syntaxType(m_type) << " : " << m_text << std::endl;
    for (auto& item : m_childList) {
        item->print(level + 1);
    }
}

SimpleASTNode::Ptr SimpleSyntaxAnalyzer::intDelare(TokenReader& tokens)
{
    SimpleASTNode::Ptr node = nullptr;
    Token token = tokens.peek();
    if (!token.isEmpty() && token.type == INT_TYPE) {
        token = tokens.read(); //consume key word "int"
        node = SimpleASTNode::create(ASTNodeType::INT_DECLARATION, token.val);
        while (!tokens.empty()) {
            if (tokens.peek().type == ID_TYPE) {
                token = tokens.read(); //consume id
                node->addChild(SimpleASTNode::create(ASTNodeType::ID, token.val));
                token = tokens.peek();
                if (!token.isEmpty() && token.type == ASSIGN_TYPE) {
                    token = tokens.read(); //consume "="
                    node->addChild(SimpleASTNode::create(ASTNodeType::ASSIGMENT, token.val));
                    SimpleASTNode::Ptr child = nullptr;
                    try {
                         child = additive(tokens);
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::terminate();
                    }

                    if (child != nullptr) {
                        node->addChild(child);
                    } else {
                        throw std::invalid_argument("invalid variable init, expect an expression!");
                    }
                }
            } else {
                throw std::invalid_argument("variable name expected!");
            }
        }
    }
    return node;
}