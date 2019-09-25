#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>

#include "front/syntax_analyze.h"
#include "utils/token_handler.h"

static SimpleASTNode::Ptr express(TokenReader& tokens);
static SimpleASTNode::Ptr orExpress(TokenReader& tokens);
static SimpleASTNode::Ptr andExpress(TokenReader& tokens);
static SimpleASTNode::Ptr equalExpress(TokenReader& tokens);
static SimpleASTNode::Ptr relExpress(TokenReader& tokens);
static SimpleASTNode::Ptr addExpress(TokenReader& tokens);
static SimpleASTNode::Ptr mulExpress(TokenReader& tokens);
static SimpleASTNode::Ptr priExpress(TokenReader& tokens);

static SimpleASTNode::Ptr additive(TokenReader& tokens);
static SimpleASTNode::Ptr multiplicative(TokenReader& tokens);
static SimpleASTNode::Ptr intLiteral(TokenReader& tokens);

/*
 * The following struct aims to save the variables
*/

std::unordered_map<std::string/*variable name*/, std::string/*variable value*/> m_varMap;
std::stack<SimpleASTNode::Ptr> m_evalStack;

static std::string syntaxType(ASTNodeType& type)
{
    switch (type) {
        case ASTNodeType::DECLARATION:
            return "Key Word";
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
        default:
            return "Default";
    }
}

// exp -> or | or = exp
static SimpleASTNode::Ptr express(TokenReader& tokens)
{
    //std::cout << "in express" << std::endl;
    SimpleASTNode::Ptr child1 = orExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    Token token = tokens.peek();
    if (node != nullptr && !token.isEmpty()) {
        if (token.type == TokenType::ASSIGN_TYPE) {
            token = tokens.read(); //consume "="
            SimpleASTNode::Ptr child2 = express(tokens);
            node = SimpleASTNode::create(ASTNodeType::ASSIGMENT, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
                throw std::invalid_argument("An valid expression is required!");
        }
    }
    return node;
}

// or -> and | or || and (BNF)
// or -> and (|| and)*   (EBNF)
static SimpleASTNode::Ptr orExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = andExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && token.type == TokenType::OR_TYPE) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = andExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::OR_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                throw std::invalid_argument("\"||\" Light argument is required!");
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// and -> equal | and && equal (BNF)
// and -> equal (&& equal)*   (EBNF)
static SimpleASTNode::Ptr andExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = equalExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && token.type == TokenType::AND_TYPE) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = equalExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::AND_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                throw std::invalid_argument("\"&&\" Light argument is required!");
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// equal -> rel | equal == rel | equal != rel (BNF)
// equal -> rel ( == | != rel)*               (EBNF)
static SimpleASTNode::Ptr equalExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = relExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && (token.type == TokenType::EQUAL_TYPE || token.type == TokenType::NEQUAL_TYPE)) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = relExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::EQUAL_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                throw std::invalid_argument("\"==\" Light argument is required!");
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// rel -> add | rel > add | rel < add | rel >= add | rel <= add (BNF)
// rel -> add ( > | < | >= | <= add)*                           (EBNF)
static SimpleASTNode::Ptr relExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = addExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && (token.type == TokenType::GLT_TYPE ||
            token.type == TokenType::GLE_TYPE)) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = addExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::REL_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                std::string str = "\"" + token.val + "\" Light argument is required!";
                throw std::invalid_argument(str.c_str());
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// add -> mul | add + mul | add - mul
// add -> mul (+ | - mul)*
static SimpleASTNode::Ptr addExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = mulExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && (token.type == TokenType::PLUS_TYPE || token.type == TokenType::MINUS_TYPE)) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = mulExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::ADDITIVE_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                std::string str = "\"" + token.val + "\" Light argument is required!";
                throw std::invalid_argument(str.c_str());
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// mul -> pri | mul * pri | mul / pri (BNF)
// mul -> pri (* | / pri)*            (EBNF)
static SimpleASTNode::Ptr mulExpress(TokenReader& tokens)
{
    SimpleASTNode::Ptr child1 = priExpress(tokens);
    SimpleASTNode::Ptr node = child1;
    while (true) {
        Token token = tokens.peek();
        if (node != nullptr && !token.isEmpty() && (token.type == TokenType::STAR_TYPE || token.type == TokenType::SLASH_TYPE)) {
            token = tokens.read();
            SimpleASTNode::Ptr child2 = priExpress(tokens);
            node = SimpleASTNode::create(ASTNodeType::MULTIL_EXP, token.val);
            node->addChild(child1);
            if (child2 != nullptr)
                node->addChild(child2);
            else
            {
                std::string str = "\"" + token.val + "\" Light argument is required!";
                throw std::invalid_argument(str.c_str());
            }
            child1 = node; //key step.
        } else {
            break;
        }
    }
    return node;
}

// pri -> Id | Liter | (exp) (BNF)
static SimpleASTNode::Ptr priExpress(TokenReader& tokens)
{
    //std::cout << "in pri express" << std::endl;
    SimpleASTNode::Ptr node = nullptr;
    Token token = tokens.peek();
    //std::cout << "token type " << token.type << std::endl;
    if (!token.isEmpty()) {
        if (token.type == TokenType::ID_TYPE || token.type == TokenType::INT_LITERAL_TYPE) {
            token = tokens.read();
            node = SimpleASTNode::create((token.type == TokenType::ID_TYPE) ? ASTNodeType::ID :
                   ASTNodeType::INT_LITERAL,  token.val);
        } else if (token.type == TokenType::LBRACKET_TYPE) {
            token = tokens.read();
            node = express(tokens);
            token = tokens.read(); // consume the ")"
            if (token.isEmpty() || token.type != TokenType::RBRACKET_TYPE)
                throw std::invalid_argument("Lack of right bracket \")\"");
        } else {
            throw std::invalid_argument("Invalid arguments, valid variable or literal is required!");
        }
    } else {
        throw std::invalid_argument("Invalid arguments, valid variable or literal is required!");
    }
    return node;
}

/*static SimpleASTNode::Ptr intLiteral(TokenReader& tokens)
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
}*/

SimpleASTNode::Ptr SimpleSyntaxAnalyzer::analyze(TokenReader& tokens)
{
    SimpleASTNode::Ptr node = nullptr;
    Token token = tokens.peek();
    if (!token.isEmpty()) {
        if (token.type == TokenType::KEY_TYPE) {
            token = tokens.read();
            node = SimpleASTNode::create(ASTNodeType::DECLARATION, token.val);
            while (!tokens.end()) {
                SimpleASTNode::Ptr child;
                child = express(tokens);
                node->addChild(child);
            }
        } else {
            node = express(tokens);
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
    if (node != nullptr)
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

void SimpleASTNode::eval(int level)
{
    //Recursive ending condition
    if (level && m_childList.empty())
        return;
    else if (!level && m_childList.empty()) {
        //std::cout << "here";
        if (m_type == ASTNodeType::ID) {
            auto search = m_varMap.find(m_text);
            if (search != m_varMap.end()) {
                std::cout << m_text << ": " << search->second;
            } else {
                std::cout << "Undefined var \'" << m_text << "\'";
            }            
        } else if (m_type == ASTNodeType::INT_LITERAL) {
            std::cout << m_text;
        }
        return;
    }
    for (auto& item : m_childList) {
        item->eval(level + 1);
        if (item->m_type == ASTNodeType::ID || item->m_type == ASTNodeType::INT_LITERAL) {
            m_evalStack.push(item);
        }
    }
    switch (m_type) {
        case ASTNodeType::DECLARATION:
            break;
        case ASTNodeType::MULTIL_EXP:
        case ASTNodeType::ADDITIVE_EXP:
        {
            SimpleASTNode::Ptr child2 = m_evalStack.top();
            m_evalStack.pop();
            SimpleASTNode::Ptr child1 = m_evalStack.top();
            m_evalStack.pop();
            int iChild1;
            int iChild2;
            if (child1->m_type == ASTNodeType::ID) {
                auto search = m_varMap.find(child1->m_text);
                if (search != m_varMap.end()) {
                    iChild1 = atoi((search->second).c_str());
                } else {
                    std::cout << "Undefined var \'" << child1->m_text << "\'";
                    break;
                }
            } else if (child1->m_type == ASTNodeType::INT_LITERAL) {
                iChild1 = atoi((child1->m_text).c_str());
            }
            if (child2->m_type == ASTNodeType::ID) {
                auto search = m_varMap.find(child2->m_text);
                if (search != m_varMap.end()) {
                    iChild2 = atoi((search->second).c_str());
                } else {
                    std::cout << "Undefined var \'" << child2->m_text << "\'";
                    break;
                }
            } else if (child2->m_type == ASTNodeType::INT_LITERAL) {
                iChild2 = atoi((child2->m_text).c_str());
            }
            int iResult = 0;
            SimpleASTNode::Ptr result;
            if (!m_text.compare({"*"})) {
                iResult = iChild1 * iChild2;
            } else if (!m_text.compare({"/"})) {
                iResult = iChild1 / iChild2;
            } else if (!m_text.compare({"+"})) {
                iResult = iChild1 + iChild2;
            } else if (!m_text.compare({"-"})) {
                iResult = iChild1 - iChild2;
            }
            result = SimpleASTNode::create(ASTNodeType::INT_LITERAL, std::to_string(iResult));
            m_evalStack.push(result);            
        }
            break;      
        case ASTNodeType::ASSIGMENT:
        {
            SimpleASTNode::Ptr child2 = m_evalStack.top();
            m_evalStack.pop();
            SimpleASTNode::Ptr child1 = m_evalStack.top();
            m_evalStack.pop();
            if (child1->m_type == ASTNodeType::ID) {
                m_varMap.insert({child1->m_text, child2->m_text});
            }
        }   
            break;
        default:
            break;     
    }

    //Handle the final results
    if (!level) {
        std::stack<SimpleASTNode::Ptr> tmp;
        while (!m_evalStack.empty()) {
            SimpleASTNode::Ptr node = m_evalStack.top();
            m_evalStack.pop();
            tmp.push(node);
        }
        while (!tmp.empty()) {
            SimpleASTNode::Ptr node = tmp.top();
            tmp.pop();
            std::cout << node->m_text;
        }
    }
}
/*SimpleASTNode::Ptr SimpleSyntaxAnalyzer::intDelare(TokenReader& tokens)
{
    SimpleASTNode::Ptr node = nullptr;
    Token token = tokens.peek();
    if (!token.isEmpty() && token.type == INT_TYPE) {
        token = tokens.read(); //consume key word "int"
        node = SimpleASTNode::create(ASTNodeType::INT_DECLARATION, token.val);
        if (tokens.empty())
            throw std::invalid_argument("variable name expected!");
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
                         child = express(tokens);
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
}*/