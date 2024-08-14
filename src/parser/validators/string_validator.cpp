#include "string_validator.h"
#include <iostream>

StringValidator::StringValidator(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {}

std::string StringValidator::validate() {
  std::string str = this->token.getValue();
  while ((this->token = this->scanner.nextToken()).getType() == TK_OPERATOR &&
         this->token.getValue() == "+") {
    this->token = this->scanner.nextToken();
    if (this->token.getType() != TK_STRING)
      throw std::runtime_error("Expected string but got: " +
                               this->token.getValue());
    str += this->token.getValue();
  }
  return str;
}
