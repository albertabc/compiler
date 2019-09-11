#ifndef __COMPILER_COMMON_H
#define __COMPILER_COMMON_H

#include <string>
// Grammar Token
enum TokenType {
    ID_TYPE,
    INT_LITERAL_TYPE,
    KEY_TYPE,
    ASSIGN_TYPE,
    GLT_TYPE,
    GLE_TYPE,
    PLUS_TYPE,
    MINUS_TYPE,
    STAR_TYPE,
    SLASH_TYPE,
    OR_TYPE,
    AND_TYPE,
    EQUAL_TYPE,
    NEQUAL_TYPE,
    LBRACKET_TYPE,
    RBRACKET_TYPE,
    NONE_TYPE,
};

struct Token {
    TokenType type;
    std::string val;
    Token():type(NONE_TYPE){}
    bool isEmpty() {return type == NONE_TYPE;}
};


template <typename T>
class Singleton
{
public:
    static T* instance()
    {
        static T obj;
        return &obj;
    }
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};
#endif