#include <iostream>
#include <string>
#include <vector>



enum Dfstate {
    Initial,
    Int_I,
    Int_N,
    Int,
    Indentifier,
    IntLiteral,
    EQ,
    GT,
    GE,
};

enum TokenType {
    Identifier_Type,
    IntLiteral_Type,
    Int_Type,
    Eq_Type,
    GT_Type,
    GE_Type,
    None_Type,
};

struct Token {
    TokenType type;
    std::string val;
};

static void handle(const std::string& src, const Dfstate& state, std::vector<Token>& res);
static void initToken(const char c, Dfstate& state, std::vector<Token>& res);
static void handleState(const char c, Dfstate& state, std::vector<Token>& res);
static std::string type2str(const TokenType& type);

static std::string type2str(const TokenType& type) {
    switch (type) {
        case Identifier_Type:
            return "Identifier";
        case IntLiteral_Type:
            return "IntLiteral";
        case Eq_Type:
            return "ASSIGN";
        case GT_Type:
            return "GT";
        case GE_Type:
            return "GE";
        case Int_Type:
            return "Int";
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

static void handle(const std::string& src, const Dfstate& initialState, std::vector<Token>& res) {
    Dfstate newState = initialState;
    for (int i = 0; i < src.length(); i++) {
        handleState(src[i], newState, res);
    }
}

static void initToken(const char c, Dfstate& state, std::vector<Token>& res) {
    if (state != Dfstate::Initial)
        return;

    Token token;
    if (c == 'i') {
        state = Dfstate::Int_I;
        token.type = TokenType::Identifier_Type;
    } else if (isAlpha(c) || c == '_') {
        state = Dfstate::Indentifier;
        token.type = TokenType::Identifier_Type;
    } else if (isDigital(c)) {
        state = Dfstate::IntLiteral;
        token.type = TokenType::IntLiteral_Type;
    } else if (c == '>') {
        state = Dfstate::GT;
        token.type = TokenType::GT_Type;
    } else if (c == '=') {
        state = Dfstate::EQ;
        token.type = TokenType::Eq_Type;
    } else {
        return;
    }

    token.val = c;
    res.push_back(token);
}

static void handleState(const char c, Dfstate& state, std::vector<Token>& res) {
    switch (state) {
        case Dfstate::Initial:
            initToken(c, state, res);
            break;
        case Dfstate::Int_I:
            if (c == 'n') {
                state = Dfstate::Int_N;
            } else {
                state = Dfstate::Indentifier;
            }
            res.back().val += c;
            break;
        case Dfstate::Int_N:
            if (c == 't') {
                state = Dfstate::Int;
            } else {
                state = Dfstate::Indentifier;
            }
            res.back().val += c;
            break;
        case Dfstate::Int:
            if (isBlank(c)) {
                res.back().type = TokenType::Int_Type;
                state = Dfstate::Initial;
                initToken(c, state, res);
            } else {
                state = Dfstate::Indentifier;
                res.back().val += c;
            }
            break;
        case Dfstate::Indentifier:
            if (isAlpha(c) || isDigital(c) || c == '_') {
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res);
            }
            break;
        case Dfstate::EQ:
            state = Dfstate::Initial;
            initToken(c, state, res);
            break;
        case Dfstate::GT:
            if (c == '=') {
                state = Dfstate::GE;
                res.back().type = TokenType::GE_Type;
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res);
            }
            break;
        case Dfstate::GE:
            state = Dfstate::Initial;
            initToken(c, state, res);
            break;
        case Dfstate::IntLiteral:
            if (isDigital(c)) {
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res); 
            }
            break;
    }
}

/* Interpret the following 3 expression
 * int age = 40, age >= 45, 2+3*5
 */
int main(int, char**) {
    std::vector<Token> res = {};
    std::string str0 = "age >= 45";
    std::string str1 = "int age = 40";
    std::string str2 = "intA_0=50";
    std::string str3 = "_int8a>=46";
    handle(str0, Dfstate::Initial, res);
    handle(str1, Dfstate::Initial, res);
    handle(str2, Dfstate::Initial, res);
    handle(str3, Dfstate::Initial, res);

    for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
        std::cout << type2str(iter->type) << ": " << iter->val << std::endl;
    return 0;
}