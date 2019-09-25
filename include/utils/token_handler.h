#ifndef __TOKEN_HANDLER_H
#define __TOKEN_HANDLER_H

#include "common.h"
#include <vector>

class TokenReader
{
public:
    TokenReader(const std::vector<Token>& tokens)
        :m_tokenList{tokens}, pos{0} {};

    Token peek();
    Token read();    
    bool empty();
    bool end();
    int getPosition();
    void setPosition(int pos);
    void unread();
private:
    std::vector<Token> m_tokenList;
    int pos;
};

#endif