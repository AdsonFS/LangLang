#include "lang_parser.h"
#include <iostream>

LangParser::LangParser(LexiScanner &_scanner, Token &_token)
    : scanner(_scanner), token(_token) {
}

void LangParser::parser() {
  this->token = this->scanner.nextToken();
  this->statementList()->solve();

  this->eof();
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
