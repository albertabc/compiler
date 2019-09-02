#include <iostream>
#include "grammar_analyze.h"

/* Interpret the following 3 expression
 * int age = 40, age >= 45, 2+3*5
 */
int main(int, char**) {
    std::vector<Token> res = {};
    std::string str0 = "age >= 45";
    std::string str1 = "int age = 40";
    std::string str2 = "intA_0=50";
    std::string str3 = "_int8a>=46";
    std::string str4 = "in arg b = 23 =b9";
    std::string str5 = "2+3   * 5";
    handle(str0, res);
    handle(str1, res);
    handle(str2, res);
    handle(str3, res);
    handle(str4, res);
    handle(str5, res);

    for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
        std::cout << type2str(iter->type) << ": " << iter->val << std::endl;
    return 0;
}