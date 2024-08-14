#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "../ast/ast.h"
#include "../lexi/lexi_scanner.h"
#include "../tokens/token.h"
/*

outputStream        : >> output outputStream*
output              : stringExpression | numericExpression

stringExpression    : string (+ stringExpression)*
string              : STRING

numericExpression   : numericExpression: term ((PLUS|MINUS) term)*
term                : factor ((DIV|MULT|MOD) factor)*
factor              : (PLUS|MINUS)factor | NUMBER | '(' numericExpression ')'
*/
class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token &_token);
  void parser();
  AST *numericExpression();
  AST *stringExpression();
  AST *outputStream();

private:
  LexiScanner &scanner;
  Token &token;

  void semiColon();
  void eof();

  bool isPlusOrMinus();
  bool isMultOrDivOrMod();

  // string expression
  AST *string();

  // numeric expression
  AST *term();
  AST *factor();
};

#endif // LANG_PARSER_H
