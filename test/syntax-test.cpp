#include <iostream>
#include "front/grammar_analyze.h"
#include "front/syntax_analyze.h"

/* Interpret the following 3 expression
 * int age = 40, age >= 45, 2+3*5
 */
int main(int, char**) {
    std::vector<Token> res = {};

    std::string str1 = "int age = 40";

    std::string str4 = "int arg, b = 23, a=3,c";

    std::string str2 = "int a = 2 + 3 *5 + 8 + 9 *100";
    std::string str5 = "int a = 2 + 3 +5";

    grammar_analyze(str5, res);
    SimpleASTNode::Ptr node;
    TokenReader reader(res);
    try {
        node = SimpleSyntaxAnalyzer::instance()->intDelare(reader);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::terminate();
    }

    for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
        std::cout << type2str(iter->type) << ": " << iter->val << std::endl;

    node->print(0);
    std::cout << (reader.empty() ? "TokenReader is empty" : "TokenReader still has items") << std::endl;
    return 0;
}