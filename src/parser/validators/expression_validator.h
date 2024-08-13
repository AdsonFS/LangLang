#ifndef EXPRESSION_VALIDATOR_H
#define EXPRESSION_VALIDATOR_H

#include "../../lexi/lexi_scanner.h"
#include <stack>

class ExpressionValidator {
public:
  ExpressionValidator(LexiScanner &scanner, Token &token);
  int validate();

private:
  LexiScanner &scanner;
  Token &token;

  int expression();
  int term();
  int factor();

  bool isPlusOrMinus();
  bool isMultOrDivOrMod();
};

#endif // EXPRESSION_VALIDATOR_H
