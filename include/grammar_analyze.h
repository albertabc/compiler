#ifndef __COMPILER_GRAMMER_ANALYZE_H
#define __COMPILER_GRAMMER_ANALYZE_H

#include <string>
#include <vector>


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


void grammar_analyze(const std::string& src, std::vector<Token>& res);
std::string type2str(const TokenType& type);
#endif