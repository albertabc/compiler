#ifndef __COMPILER_GRAMMER_ANALYZE_H
#define __COMPILER_GRAMMER_ANALYZE_H

#include <string>
#include <vector>
#include "common.h"




void grammar_analyze(const std::string& src, std::vector<Token>& res);
std::string type2str(const TokenType& type);
#endif