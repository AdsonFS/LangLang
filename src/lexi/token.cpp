#include "token.h"

Token::Token(TokenType type, std::string value) {
  this->type = type;
  this->value = value;
}

