#include <iostream>
#include "front/grammar_analyze.h"
#include "front/syntax_analyze.h"

/* Interpret the following 3 expression
 * int age = 40, age >= 45, 2+3*5
 */
int main(int, char**) {
    std::vector<Token> res = {};

    std::vector<std::string> testCases = {"int age = 40", "int arg, b = 23, a=3,c", "int a = 2 + 3 *5 + 8 + 9 *100", \
    "int a = 2 + 3 +5", "int", "int "};

    for (auto& item : testCases) {
        grammar_analyze(item, res);
        for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
            std::cout << type2str(iter->type) << ": " << iter->val << std::endl;
        SimpleASTNode::Ptr node;
        TokenReader reader(res);
        try {
            node = SimpleSyntaxAnalyzer::instance()->intDelare(reader);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::terminate();
        }

        if (node != nullptr)
            node->print(0);
        res.erase(res.begin(), res.end());
        std::cout << (reader.empty() ? "TokenReader is empty\n" : "TokenReader still has items\n") << std::endl;
    }
    return 0;
}