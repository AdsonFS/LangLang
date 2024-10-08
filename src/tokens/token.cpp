#include "token.h"

Token::Token() : type(TK_UNKNOWN), value(""), position(Position(1, 1, 1)) {}
Token::Token(TokenType type, std::string value) : type(type), value(value), position(Position(1, 1, 1)) {}
Token::Token(TokenType type, std::string value, Position position) 
  : type(type), value(value), position(position) {}

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
    case TK_CURLY_BRACES:
      return "CURLY_BRACES";
    case TK_COMPARATOR:
      return "COMPARATOR";
    case TK_LOGICAL_OPERATOR:
      return "LOGICAL_OPERATOR";
    case TK_EQUALITY_OPERATOR:
      return "EQUALITY_OPERATOR";
    case TK_COMMENT:
      return "COMMENT";
    case TK_DOT:
      return "DOT";
    case TK_ARROW:
      return "ARROW";
    case TK_COLON:
      return "COLON";
      break;
    }
  return "";
}
