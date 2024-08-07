#include "lang_parser.h"
#include <iostream>

LangParser::LangParser(LexiScanner &_scanner, Token _token)
    : scanner(_scanner), token(_token) {}

Token LangParser::nextToken() {
  return this->token = this->scanner.nextToken();
}

void LangParser::Parser() {
  this->nextToken();

  switch (this->token.getType()) {
  case TokenType::TK_RESERVED_WORD:
    if (this->token.getValue() == "LOUT")
      this->Lout();
    else
      throw std::runtime_error("Syntax error");
    break;
  default:
    throw std::runtime_error("Syntax error!");
  }

  /*this->ArithmeticExpression();*/
  this->EoF();
}

void LangParser::Lout() {
  this->nextToken();
  if (this->token.getType() != TokenType::TK_NUMBER)
    throw std::runtime_error("Syntax error: NUMBER");
  std::cout << this->token.getValue() << std::endl;
  this->nextToken();
  this->semiColon();
}

void LangParser::semiColon() {
  if (this->token.getType() != TokenType::TK_SEMICOLON)
    throw std::runtime_error("Syntax error: expected SEMICOLON");
}

void LangParser::EoF() {
  if (this->nextToken().getType() != TokenType::TK_EOF)
    throw std::runtime_error("Syntax error: expected EOF");
}

void LangParser::arithmeticExpression() { this->E(); }

void LangParser::E() {
  this->T();
  this->E_();
}

void LangParser::T() {
  this->nextToken();
  if (this->token.getType() != TokenType::TK_IDENTIFIER &&
      this->token.getType() != TokenType::TK_NUMBER)
    throw std::runtime_error("Syntax error: expected ID or NUMBER");
}

void LangParser::E_() {
  this->nextToken();
  if (this->token.getType() == TokenType::TK_EOF ||
      this->token.getType() == TokenType::TK_SEMICOLON)
    return;

  this->OP();
  this->T();
  this->E_();
}

void LangParser::OP() {
  if (this->token.getType() != TokenType::TK_OPERATOR)
    throw std::runtime_error("Syntax error: expected OPERATOR -> " +
                             this->token.getValue());
}
