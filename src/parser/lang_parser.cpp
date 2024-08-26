#include "lang_parser.h"
#include "../ast/ast_printer.h"

LangParser::LangParser(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {}

AST* LangParser::parser() {
  this->token = this->scanner.nextToken();
  AST* ast = this->statementList();
  this->eof();
  return ast;
}

Token LangParser::consume(Token expectedToken) {
  Token consumed = this->token;
  if (this->token.getType() != expectedToken.getType())
    throw std::runtime_error("Syntax error: expected " +
                             expectedToken.toString());
  if (expectedToken.getValue() != "" &&
      this->token.getValue() != expectedToken.getValue())
    throw std::runtime_error("Syntax error: expected " +
                             expectedToken.getValue());
  this->token = this->scanner.nextToken();
  return consumed;
}

void LangParser::semiColon() {
  if (this->token.getType() != TokenType::TK_SEMICOLON)
    throw std::runtime_error("Syntax error: expected SEMICOLON");
}

void LangParser::eof() {
  if (this->scanner.nextToken().getType() != TokenType::TK_EOF)
    throw std::runtime_error("Syntax error: expected EOF");
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
