#ifndef __TOKEN_HANDLER_H
#define __TOKEN_HANDLER_H

#include "common.h"
#include <vector>

class TokenReader
{
public:
    TokenReader(const std::vector<Token>& tokens)
        :m_tokenList{tokens} {};

    Token peek();
    Token read();
    bool empty();
private:
    std::vector<Token> m_tokenList;
};

#endif