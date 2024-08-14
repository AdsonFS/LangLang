#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "../../lexi/lexi_scanner.h"
#include "../../ast/ast.h"
#include <stack>

class ExpressionParser {
public:
  ExpressionParser(LexiScanner &scanner, Token &token);
  AST * parser();
private:
  LexiScanner &scanner;
  Token &token;

  AST* expression();
  AST* term();
  AST* factor();

  bool isPlusOrMinus();
  bool isMultOrDivOrMod();
};

#endif // EXPRESSION_PARSER_H
