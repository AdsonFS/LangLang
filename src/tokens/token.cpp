#include "token.h"

Token::Token() : type(TK_UNKNOWN), value("") {}
Token::Token(TokenType type, std::string value) {
  this->type = type;
  this->value = value;
}

std::string Token::toString() {
  switch(this->type) {
    case TK_IDENTIFIER:
      return "IDENTIFIER";
    case TK_STRING:
      return "STRING";
    case TK_NUMBER:
      return "NUMBER";
    case TK_OPERATOR:
      return "OPERATOR";
    case TK_PARENTHESES:
      return "PARENTHESES";
    case TK_OUTPUTSTREAM:
      return "OUTPUTSTREAM";
    case TK_RESERVED_WORD:
      return "RESERVED_WORD";
    case TK_ASSIGNMENT:
      return "ASSIGNMENT";
    case TK_SEMICOLON:
      return "SEMICOLON";
    case TK_UNKNOWN:
      return "UNKNOWN";
    case TK_EOF:
      return "EOF";
    case TK_INPUTSTREAM:
      return "INPUTSTREAM";
    }
  return "";
}
