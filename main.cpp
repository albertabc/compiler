#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>



enum Dfstate {
    INITIAL,
    INT_I,
    INT_N,
    INT,
    ID,
    INT_LITERAL,
    OPERAOTR,
    EQ,
    GT,
    GE,
};

enum TokenType {
    ID_TYPE,
    INT_LITERAL_TYPE,
    INT_TYPE,
    EQ_TYPE,
    GT_TYPE,
    GE_TYPE,
    PLUS_TYPE,
    MINUS_TYPE,
    STAR_TYPE,
    SLASH_TYPE,
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

static std::unordered_map<char, TokenType> tokenMap = {{'+', PLUS_TYPE}, {'-', MINUS_TYPE}, {'*', STAR_TYPE}, {'/', SLASH_TYPE}};

static std::string type2str(const TokenType& type) {
    switch (type) {
        case ID_TYPE:
            return "Identifier";
        case INT_LITERAL_TYPE:
            return "IntLiteral";
        case EQ_TYPE:
            return "ASSIGN";
        case GT_TYPE:
            return "GT";
        case GE_TYPE:
            return "GE";
        case INT_TYPE:
            return "Int";
        case PLUS_TYPE:
            return "Plus";
        case MINUS_TYPE:
            return "Minus";
        case STAR_TYPE:
            return "Star";
        case SLASH_TYPE:
            return "Slash";
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
    if (state != INITIAL)
        return;

    Token token;
    if (c == 'i') {
        state = INT_I;
        token.type = ID_TYPE;
    } else if (isAlpha(c) || c == '_') {
        state = ID;
        token.type = ID_TYPE;
    } else if (isDigital(c)) {
        state = INT_LITERAL;
        token.type = INT_LITERAL_TYPE;
    } else if (c == '>') {
        state = GT;
        token.type = GT_TYPE;
    } else if (c == '=') {
        state = EQ;
        token.type = EQ_TYPE;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
        state = OPERAOTR;
        token.type = tokenMap[c];
    } else {
        return;
    }

    token.val = c;
    res.push_back(token);
}

static void handleState(const char c, Dfstate& state, std::vector<Token>& res) {
    switch (state) {
        case INITIAL:
        case EQ:
        case OPERAOTR:
            state = INITIAL;
            initToken(c, state, res);
            break;
        case INT_I:
            if (c == 'n') {
                state = INT_N;
            } else {
                state = ID;
            }
            res.back().val += c;
            break;
        case INT_N:
            if (c == 't') {
                state = INT;
            } else {
                state = ID;
            }
            res.back().val += c;
            break;
        case INT:
            if (isBlank(c)) {
                res.back().type = INT_TYPE;
                state = INITIAL;
                initToken(c, state, res);
            } else {
                state = ID;
                res.back().val += c;
            }
            break;
        case ID:
            if (isAlpha(c) || isDigital(c) || c == '_') {
                res.back().val += c;
            } else {
                state = INITIAL;
                initToken(c, state, res);
            }
            break;
        case GT:
            if (c == '=') {
                state = GE;
                res.back().type = GE_TYPE;
                res.back().val += c;
            } else {
                state = INITIAL;
                initToken(c, state, res);
            }
            break;
        case GE:
            state = INITIAL;
            initToken(c, state, res);
            break;
        case INT_LITERAL:
            if (isDigital(c)) {
                res.back().val += c;
            } else {
                state = INITIAL;
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
    //std::string str4 = "in arg b = 23 =b9";
    std::string str5 = "2+3   * 5";
    handle(str0, INITIAL, res);
    handle(str1, INITIAL, res);
    handle(str2, INITIAL, res);
    handle(str3, INITIAL, res);
    //handle(str4, INITIAL, res);
    handle(str5, INITIAL, res);

    for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
        std::cout << type2str(iter->type) << ": " << iter->val << std::endl;
    return 0;
}