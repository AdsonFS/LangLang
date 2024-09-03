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
