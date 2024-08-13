#include "lang_parser.h"
#include "validators/expression_validator.h"
#include "validators/string_validator.h"
#include <iostream>

LangParser::LangParser(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {}

void LangParser::parser() {
  this->token = this->scanner.nextToken();
  std::cout << "Result: "
            << ExpressionValidator(this->scanner, this->token).validate()
            << std::endl;
  /*switch (this->token.getType()) {*/
  /*case TokenType::TK_STRING:*/
  /*    std::cout << StringValidator(this->scanner, this->token).validate() <<
   * std::endl;*/
  /*    this->semiColon();*/
  /*  break;*/
  /*case TokenType::TK_NUMBER:*/
  /*  std::cout << ExpressionValidator(this->scanner, this->token).validate() <<
   * std::endl;*/
  /*  this->semiColon();*/
  /*  break;*/
  /*default:*/
  /*  throw std::runtime_error("Syntax error");*/
  /*}*/
  /*this->semiColon();*/
  /*this->ArithmeticExpression();*/
  /*this->eof();*/
}

/*void LangParser::Lout() {*/
/*this->nextToken();*/
/*if (this->token.getType() != TokenType::TK_NUMBER)*/
/*  throw std::runtime_error("Syntax error: NUMBER");*/
/*std::cout << this->token.getValue() << std::endl;*/
/*this->nextToken();*/
/*this->semiColon();*/
/*}*/

void LangParser::semiColon() {
  if (this->token.getType() != TokenType::TK_SEMICOLON)
    throw std::runtime_error("Syntax error: expected SEMICOLON");
}

void LangParser::eof() {
  if (this->scanner.nextToken().getType() != TokenType::TK_EOF)
    throw std::runtime_error("Syntax error: expected EOF");
}

/*void LangParser::arithmeticExpression() { this->E(); }*/

/*void LangParser::E() {*/
/*this->T();*/
/*this->E_();*/
/*}*/

/*void LangParser::T() {*/
/*  this->nextToken();*/
/*  if (this->token.getType() != TokenType::TK_IDENTIFIER &&*/
/*      this->token.getType() != TokenType::TK_NUMBER)*/
/*    throw std::runtime_error("Syntax error: expected ID or NUMBER");*/
/*}*/

/*void LangParser::E_() {*/
/*this->nextToken();*/
/*if (this->token.getType() == TokenType::TK_EOF ||*/
/*    this->token.getType() == TokenType::TK_SEMICOLON)*/
/*  return;*/
/**/
/*this->OP();*/
/*this->T();*/
/*this->E_();*/
/*}*/

/*void LangParser::OP() {*/
/*if (this->token.getType() != TokenType::TK_OPERATOR) throw
 * std::runtime_error("Syntax error: expected OPERATOR -> " +*/
/*                           this->token.getValue());*/
/*}*/
