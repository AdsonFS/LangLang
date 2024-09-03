#include "../lang_parser.h"

AST *LangParser::ifStatement() {
  this->consume(Token(TK_RESERVED_WORD, "if"));
  this->consume(Token(TK_PARENTHESES, "("));
  AST *conditional = this->expression();
  this->consume(Token(TK_PARENTHESES, ")"));
  this->consume(Token(TK_CURLY_BRACES, "{"));
  AST *statementList = this->statementList();
  this->consume(Token(TK_CURLY_BRACES, "}"));

  AST* elseStatementList = nullptr;
  
  if(this->match(Token(TK_RESERVED_WORD, "else"))) {
    this->consume(Token(TK_RESERVED_WORD, "else"));
    this->consume(Token(TK_CURLY_BRACES, "{"));
    elseStatementList = this->statementList();
    this->consume(Token(TK_CURLY_BRACES, "}"));
  }
  return new IfStatementAST(conditional,
                            dynamic_cast<StatementListAST *>(statementList),
                            dynamic_cast<StatementListAST *>(elseStatementList));
}
AST *LangParser::whileStatement() {
  this->consume(Token(TK_RESERVED_WORD, "while"));
  this->consume(Token(TK_PARENTHESES, "("));
  AST *conditional = this->expression();
  this->consume(Token(TK_PARENTHESES, ")"));
  this->consume(Token(TK_CURLY_BRACES, "{"));
  AST *statementList = this->statementList();
  this->consume(Token(TK_CURLY_BRACES, "}"));

  return new WhileStatementAST(conditional,
                               dynamic_cast<StatementListAST *>(statementList));
}

AST *LangParser::forStatement() {
  AST* init, *conditional, *increment, *statementList;

  this->consume(Token(TK_RESERVED_WORD, "for"));
  this->consume(Token(TK_PARENTHESES, "("));
  
  if(this->match(Token(TK_RESERVED_WORD, "var")))
    init = this->variableDeclaration();
  else init = this->expression();
  this->consume(Token(TK_SEMICOLON, ""));


  conditional = this->expression();
  this->consume(Token(TK_SEMICOLON, ""));
  increment = this->expression();
  this->consume(Token(TK_PARENTHESES, ")"));
  this->consume(Token(TK_CURLY_BRACES, "{"));
  statementList = this->statementList();
  this->consume(Token(TK_CURLY_BRACES, "}"));

  return new ForStatementAST(init, conditional, increment,
                             dynamic_cast<StatementListAST *>(statementList));
}
