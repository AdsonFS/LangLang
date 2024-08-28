#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "../ast/ast.h"
#include "../lexi/lexi_scanner.h"
#include "../tokens/token.h"
#include "../error/error.h"

class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token &_token);
  AST* parser();
  AST *statement();
  AST *assignment();
  AST *inputStream();
  AST *ifStatement();
  AST *conditional();
  AST *outputStream();
  AST *statementList();
  AST *whileStatement();
  AST *statementFunction();
  AST *variableDeclaration();
  AST *expression();
  AST *equalityExpression();
  AST *logicalExpression(); 

private:
  LexiScanner &scanner;
  Token &token;

  void semiColon();
  void eof();

  Token consume(Token expectedToken);

  bool isComparator();
  bool isPlusOrMinus();
  bool isMultOrDivOrMod();
  bool isLogicalOperator();
  bool isEqualityOperator();

  AST *term();
  AST *clause();
  AST *factor();
};

#endif // LANG_PARSER_H
