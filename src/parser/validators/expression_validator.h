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

  int procedence(char op);
  void applyOpration(std::stack<int> &values, char op);
};

#endif // EXPRESSION_VALIDATOR_H
