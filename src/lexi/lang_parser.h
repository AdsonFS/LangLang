#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "lexi_scanner.h"
#include "token.h"

class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token _token);
  void Parser();
private:
  LexiScanner &scanner;
  Token token;
  
  void SemiColon();
  void EoF();
  void ArithmeticExpression();
  void E();
  void T();
  void OP();

  void E_();
};

#endif // LANG_PARSER_H
