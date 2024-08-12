#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
  /*TK_IDENTIFIER,*/
  TK_STRING,
  TK_NUMBER,
  TK_OPERATOR,
  /*TK_RESERVED_WORD,*/
  /*TK_CMP_OPERATOR,*/
  /*TK_WHITESPACE,*/
  TK_SEMICOLON,
  TK_UNKNOWN,
  TK_EOF
};

class Token {
public:
  Token();
  Token(TokenType type, std::string value);
  TokenType getType() { return this->type; }
  std::string getValue() { return this->value; }
private:
  TokenType type;
  std::string value;
};

#endif // TOKEN_H
