#include <iostream>
#include "front/grammar_analyze.h"
#include "front/syntax_analyze.h"


/*
 * This is the main entry of myself REPL (Read-Eval-Print-Language)
 * It will realize the following functions:
 * 1. An interactive interface for input and output
 * 2. Caculate the the result of expression
 *    (1). Assignment expression
 *    (2). Equal/Relative expression
 *    (3). Additive expression
*/

int main(int argc, char** argv)
{
    std::cout << "\n>>>";
    bool verbose = argv[1] ? true : false;
    std::string scriptText;
    std::vector<Token> res;

    while (true) {
        std::getline(std::cin, scriptText);
        if (!scriptText.compare(std::string{"exit()"})) {
            std::cout << "Goodbye" << std::endl;
            break;
        } else if (scriptText.back() == ';') {
            //std::cout << scriptText << std::endl;
            grammar_analyze(scriptText, res);
            //for (std::vector<Token>::iterator iter = res.begin(); iter != res.end(); iter++)
            //    std::cout << type2str(iter->type) << ": " << iter->val << std::endl;
            SimpleASTNode::Ptr node;
            TokenReader reader(res);
            try {
                node = SimpleSyntaxAnalyzer::instance()->analyze(reader);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }

            if (verbose && node != nullptr)
                node->print(0);
            if (node != nullptr)
                node->eval(0);
            res.erase(res.begin(), res.end());
            //std::cout << (reader.empty() ? "TokenReader is empty\n" : "TokenReader still has items\n") << std::endl;
            std::cout << "\n>>>";
        }
    }

    return 0;
}