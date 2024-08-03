#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
  TK_IDENTIFIER,
  TK_NUMBER,
  TK_OPERATOR,
  TK_CMP_OPERATOR,
  TK_WHITESPACE,
  TK_SEMICOLON,
  TK_UNKNOWN,
  TK_EOF
};

class Token {
public:
  Token(TokenType type, std::string value);
  TokenType getType() { return this->type; }
  std::string getValue() { return this->value; }
private:
  TokenType type;
  std::string value;
};

#endif // TOKEN_H
