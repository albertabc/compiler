#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "front/grammar_analyze.h"


enum class Dfstate {
    INITIAL,
    INT_I,
    INT_N,
    INT,
    ID,
    INT_LITERAL,
    OPERATOR,
    ASSIGN,
    GLT, // Greater or less than
    GLE, // >= or <=
    BRACKET,
};

static void initToken(const char c, Dfstate& state, std::vector<Token>& res);
static void handleState(const char c, Dfstate& state, std::vector<Token>& res);

static std::unordered_map<char, TokenType> tokenMap = {{'+', PLUS_TYPE}, {'-', MINUS_TYPE}, {'*', STAR_TYPE}, {'/', SLASH_TYPE}};

std::string type2str(const TokenType& type) {
    switch (type) {
        case ID_TYPE:
            return "Identifier";
        case INT_LITERAL_TYPE:
            return "IntLiteral";
        case ASSIGN_TYPE:
            return "ASSIGN";
        case GLT_TYPE:
            return "GLT";
        case GLE_TYPE:
            return "GLE";
        case KEY_TYPE:
            return "Key";
        case PLUS_TYPE:
            return "Plus";
        case MINUS_TYPE:
            return "Minus";
        case STAR_TYPE:
            return "Star";
        case SLASH_TYPE:
            return "Slash";
        case LBRACKET_TYPE:
            return "LBracket";
        case RBRACKET_TYPE:
            return "RBracket";
        default:
            return "None";
    }
}

static bool isAlpha(const char& c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

static bool isDigital(const char& c) {
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

static bool isBlank(const char& c) {
    if (c == ' ' || c == '\t') {
        return true;
    }
    return false;
}

void grammar_analyze(const std::string& src, std::vector<Token>& res) {
    Dfstate newState = Dfstate::INITIAL;
    for (int i = 0; i < src.length(); i++) {
        handleState(src[i], newState, res);
    }
}

static void initToken(const char c, Dfstate& state, std::vector<Token>& res) {
    if (state != Dfstate::INITIAL)
        return;

    Token token;
    if (c == 'i') {
        state = Dfstate::INT_I;
    } else if (isAlpha(c) || c == '_') {
        state = Dfstate::ID;
        token.type = ID_TYPE;
    } else if (isDigital(c)) {
        state = Dfstate::INT_LITERAL;
        token.type = INT_LITERAL_TYPE;
    } else if (c == '>' || c == '<') {
        state = Dfstate::GLT;
        token.type = GLT_TYPE;
    } else if (c == '=') {
        state = Dfstate::ASSIGN;
        token.type = ASSIGN_TYPE;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
        state = Dfstate::OPERATOR;
        token.type = tokenMap[c];
    } else if (c == '(' || c == ')'){
        state = Dfstate::BRACKET;
        token.type = (c == '(') ? LBRACKET_TYPE : RBRACKET_TYPE;
    } else {
        return;
    }

    token.val = c;
    res.push_back(token);
}

static void handleState(const char c, Dfstate& state, std::vector<Token>& res) {
    switch (state) {
        case Dfstate::INITIAL:
        case Dfstate::ASSIGN:
        case Dfstate::OPERATOR:
        case Dfstate::BRACKET:
            state = Dfstate::INITIAL;
            initToken(c, state, res);
            break;
        case Dfstate::INT_I:
            if (c == 'n') {
                state = Dfstate::INT_N;
                res.back().val += c;
            } else {
                state = Dfstate::ID;
                if (isAlpha(c) || isDigital(c) || c == '_') {
                    res.back().val += c;
                    res.back().type = ID_TYPE;
                } else {
                    state = Dfstate::INITIAL;
                    initToken(c, state, res);
                }
            }
            break;
        case Dfstate::INT_N:
            if (c == 't') {
                state = Dfstate::INT;
                res.back().val += c;
                res.back().type = KEY_TYPE;
            } else {
                state = Dfstate::ID;
                if (isAlpha(c) || isDigital(c) || c == '_') {
                    res.back().val += c;
                    res.back().type = ID_TYPE;
                } else {
                    state = Dfstate::INITIAL;
                    initToken(c, state, res);
                }
            }
            break;
        case Dfstate::INT:
            if (isBlank(c)) {
                state = Dfstate::INITIAL;
                initToken(c, state, res);
            } else {
                state = Dfstate::ID;
                res.back().val += c;
                res.back().type = ID_TYPE;
            }
            break;
        case Dfstate::ID:
            if (isAlpha(c) || isDigital(c) || c == '_') {
                res.back().val += c;
            } else {
                state = Dfstate::INITIAL;
                initToken(c, state, res);
            }
            break;
        case Dfstate::GLT:
            if (c == '=') {
                state = Dfstate::GLE;
                res.back().type = GLE_TYPE;
                res.back().val += c;
            } else {
                state = Dfstate::INITIAL;
                initToken(c, state, res);
            }
            break;
        case Dfstate::GLE:
            state = Dfstate::INITIAL;
            initToken(c, state, res);
            break;
        case Dfstate::INT_LITERAL:
            if (isDigital(c)) {
                res.back().val += c;
            } else {
                state = Dfstate::INITIAL;
                initToken(c, state, res); 
            }
            break;
    }
}