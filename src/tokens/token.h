#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
  TK_IDENTIFIER,
  TK_DOT,
  TK_COLON,
  TK_STRING,
  TK_NUMBER,
  TK_ARROW,
  TK_OPERATOR,
  TK_PARENTHESES,
  TK_INPUTSTREAM,
  TK_OUTPUTSTREAM,
  TK_RESERVED_WORD,
  TK_ASSIGNMENT,
  TK_CURLY_BRACES,
  TK_COMPARATOR,
  TK_LOGICAL_OPERATOR,
  TK_EQUALITY_OPERATOR,
  /*TK_CMP_OPERATOR,*/
  /*TK_WHITESPACE,*/
  TK_SEMICOLON,
  TK_UNKNOWN,
  TK_COMMENT,
  TK_EOF
};

class Position {
public:
  Position(int line, int column, int position)
    : line(line), column(column), position(position) {}
  int getLine() { return this->line; }
  int getColumn() { return this->column; }
  int getPosition() { return this->position; }
private:
  int line, column, position;
};

class Token {
public:
  Token();
  Token(TokenType type, std::string value);
  Token(TokenType type, std::string value, Position position);
  TokenType getType() { return this->type; }
  std::string getValue() { return this->value; }
  std::string toString();
  Position getPosition() { return this->position; }
private:
  TokenType type;
  std::string value;
  Position position;
};


#endif // TOKEN_H
