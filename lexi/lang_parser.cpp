
#include "lang_parser.h"
#include "token.h"
LangParser::LangParser(LexiScanner &_scanner, Token _token)
    : scanner(_scanner), token(_token) {}

void LangParser::Parser() {
  this->ArithmeticExpression();
  this->SemiColon();
  this->EoF();
}

void LangParser::SemiColon() {
  if (this->token.getType() != TokenType::TK_SEMICOLON)
    throw std::runtime_error("Syntax error: expected SEMICOLON");
}

void LangParser::EoF() {
  if (this->scanner.nextToken().getType() != TokenType::TK_EOF)
    throw std::runtime_error("Syntax error: expected EOF");
}

void LangParser::ArithmeticExpression() { this->E(); }

void LangParser::E() {
  this->T();
  this->E_();
}

void LangParser::T() {
  this->token = this->scanner.nextToken();
  if (this->token.getType() != TokenType::TK_IDENTIFIER &&
      this->token.getType() != TokenType::TK_NUMBER)
    throw std::runtime_error("Syntax error: expected ID or NUMBER");
}

void LangParser::E_() {
  this->token = this->scanner.nextToken();
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
