#include "utils/token_handler.h"

Token TokenReader::peek()
{
    Token token;
    if (!m_tokenList.empty() && pos < m_tokenList.size()) {
        token = m_tokenList[pos];
    }
    return token;
}

Token TokenReader::read()
{
    Token token;
    /*std::vector<Token>::iterator iter = m_tokenList.begin();
    if (iter != m_tokenList.end()) {
        token = *iter;
        m_tokenList.erase(iter);
    }*/
    //Modify the implementation of this method mainly because backtrack is supported
    //So the "read" cannot really erase the item, just a position movement.
    if (pos < m_tokenList.size())
        token = m_tokenList[pos++];
    return token;
}

void TokenReader::unread()
{
    pos--;
}

bool TokenReader::end()
{
    return (pos >= m_tokenList.size() - 1) ? true : false;
}

bool TokenReader::empty()
{
    return m_tokenList.empty();
}

int TokenReader::getPosition()
{
    return pos;
}

void TokenReader::setPosition(int pos)
{
    this->pos = pos;
}