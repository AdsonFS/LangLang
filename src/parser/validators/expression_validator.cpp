#include "expression_validator.h"
#include <iostream>

ExpressionValidator::ExpressionValidator(LexiScanner &scanner, Token &token)
    : scanner(scanner), token(token) {}

int ExpressionValidator::validate() {
  std::cout << "ExpressionValidator::validate()" << std::endl;
  int result = stoi(this->token.getValue());
  while ((this->token = this->scanner.nextToken()).getType() == TK_OPERATOR) {
    if (this->token.getValue() != "+")
      throw std::runtime_error("Expected operator '+' but got: " +
                               this->token.getValue());
    this->token = this->scanner.nextToken();
    if (this->token.getType() != TK_NUMBER)
      throw std::runtime_error("Expected number but got: " +
                               this->token.getValue());
    result += std::stoi(this->token.getValue());
  }
  return result;
}
