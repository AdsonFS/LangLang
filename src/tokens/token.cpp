#include "token.h"

Token::Token() : type(TK_UNKNOWN), value("") {}
Token::Token(TokenType type, std::string value) {
  this->type = type;
  this->value = value;
}
