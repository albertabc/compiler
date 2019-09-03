#include "utils/token_handler.h"

Token TokenReader::peek()
{
    Token token;
    if (!m_tokenList.empty()) {
        token = m_tokenList.front();
    }
    return token;
}

Token TokenReader::read()
{
    Token token;
    std::vector<Token>::iterator iter = m_tokenList.begin();
    if (iter != m_tokenList.end()) {
        token = *iter;
        m_tokenList.erase(iter);
    }
    return token;
}

bool TokenReader::empty()
{
    return m_tokenList.empty();
}