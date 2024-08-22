#ifndef LANG_PARSER_H
#define LANG_PARSER_H

#include "../ast/ast.h"
#include "../lexi/lexi_scanner.h"
#include "../tokens/token.h"

class LangParser {
public:
  LangParser(LexiScanner &_scanner, Token &_token);
  void parser();
  AST *statement();
  AST *assignment();
  AST *expression();
  AST *inputStream();
  AST *ifStatement();
  AST *conditional();
  AST *outputStream();
  AST *statementList();
  AST *variableDeclaration();
  AST *statementFunction();

private:
  LexiScanner &scanner;
  Token &token;

  void semiColon();
  void eof();

  Token consume(Token expectedToken);

  bool isPlusOrMinus();
  bool isMultOrDivOrMod();

  AST *term();
  AST *factor();
};

#endif // LANG_PARSER_H
