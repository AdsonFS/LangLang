#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
  TK_IDENTIFIER,
  TK_STRING,
  TK_NUMBER,
  TK_OPERATOR,
  TK_PARENTHESES,
  TK_INPUTSTREAM,
  TK_OUTPUTSTREAM,
  TK_RESERVED_WORD,
  TK_ASSIGNMENT,
  TK_CURLY_BRACES,
  TK_COMPARATOR,
  TK_LOGICAL_OPERATOR,
  /*TK_CMP_OPERATOR,*/
  /*TK_WHITESPACE,*/
  TK_SEMICOLON,
  TK_UNKNOWN,
  TK_COMMENT,
  TK_EOF
};

class Token {
public:
  Token();
  Token(TokenType type, std::string value);
  TokenType getType() { return this->type; }
  std::string getValue() { return this->value; }
  std::string toString();
private:
  TokenType type;
  std::string value;
};

#endif // TOKEN_H
