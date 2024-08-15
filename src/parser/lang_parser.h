#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "../ast/ast.h"
#include "../lexi/lexi_scanner.h"
#include "../tokens/token.h"

class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token &_token);
  void parser();
  AST *numericExpression();
  AST *stringExpression();
  AST *outputStream();
  AST *statementList();
  AST *statement();
  AST *variableDeclaration();

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
