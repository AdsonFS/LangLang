#include "lang_parser.h"
#include "../error/error.h"

LangParser::LangParser(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {}

AST *LangParser::parser() {
  this->token = this->scanner.nextToken();
  AST *ast = this->statementList();
  this->eof();
  return ast;
}

Token LangParser::consume(Token expectedToken) {
  Token token = this->token;
  if (!this->match(expectedToken))
    throw SyntaxError(token, expectedToken.toString());
  this->token = this->scanner.nextToken();
  return token;
}

Token LangParser::consume(TokenType expectedToken) {
  Token token = this->token;
  if (!this->match(expectedToken))
    throw SyntaxError(token, Token(expectedToken, "").toString());
  this->token = this->scanner.nextToken();
  return token;
}

bool LangParser::match(Token expectedToken) {
  if (this->token.getType() != expectedToken.getType())
    return false;
  return this->token.getValue() == expectedToken.getValue();
}

bool LangParser::match(TokenType expectedToken) {
  return this->token.getType() == expectedToken;
}

void LangParser::semiColon() {
  if (this->token.getType() != TokenType::TK_SEMICOLON)
    throw SyntaxError(this->token, "SEMICOLON");
}

void LangParser::eof() {
  if (this->scanner.nextToken().getType() != TokenType::TK_EOF)
    throw SyntaxError(this->token, "a statement");
}

bool LangParser::isEqualityOperator() {
  return this->token.getType() == TokenType::TK_EQUALITY_OPERATOR;
}

bool LangParser::isLogicalOperator() {
  return this->token.getType() == TokenType::TK_LOGICAL_OPERATOR;
}

bool LangParser::isComparator() {
  return this->token.getType() == TokenType::TK_COMPARATOR;
}

bool LangParser::isPlusOrMinus() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "+" || this->token.getValue() == "-");
}

bool LangParser::isMultOrDivOrMod() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "*" || this->token.getValue() == "/" ||
          this->token.getValue() == "%");
}
