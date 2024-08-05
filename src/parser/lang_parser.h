#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "../lexi/lexi_scanner.h"
#include "../tokens/token.h"
#include "validators/lout_validator.h"

class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token _token);
  void Parser();
private:
  LexiScanner &scanner;
  Token token;

  

  Token nextToken();
  void semiColon();
  void EoF();
  void arithmeticExpression();
  void E();
  void T();
  void OP();

  void E_();
  void Lout();
};

#endif // LANG_PARSER_H
