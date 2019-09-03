#ifndef __COMPILER_COMMON_H
#define __COMPILER_COMMON_H

#include <string>
// Grammar Token
enum TokenType {
    ID_TYPE,
    INT_LITERAL_TYPE,
    INT_TYPE,
    ASSIGN_TYPE,
    GT_TYPE,
    GE_TYPE,
    PLUS_TYPE,
    MINUS_TYPE,
    STAR_TYPE,
    SLASH_TYPE,
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