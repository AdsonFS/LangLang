#include "lang_parser.h"
#include <iostream>

LangParser::LangParser(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {}

void LangParser::parser() {
  this->token = this->scanner.nextToken();
  AST* ast = this->outputStream();
  int result = std::get<int>(ast->solve());
  std::cout << "Result: " << result << std::endl;
  /*AST* ast = this->numericExpression();*/
  /*std::cout << "Result: "*/
  /*          << std::get<int>(ast->solve())*/
  /*          << std::endl;*/
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
  this->semiColon();
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

bool LangParser::isPlusOrMinus() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "+" || this->token.getValue() == "-");
}

bool LangParser::isMultOrDivOrMod() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "*" || this->token.getValue() == "/" ||
          this->token.getValue() == "%");
}
