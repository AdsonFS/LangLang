#ifndef EXPRESSION_VALIDATOR_H
#define EXPRESSION_VALIDATOR_H

#include "../../lexi/lexi_scanner.h"

class ExpressionValidator {
public:
  ExpressionValidator(LexiScanner &scanner, Token &token);
  int validate();

private:
  LexiScanner &scanner;
  Token &token;
};

#endif // EXPRESSION_VALIDATOR_H
