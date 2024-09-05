#include "../lang_parser.h"

AST *LangParser::statementList() {
  std::vector<AST *> statements;
  while (this->token.getType() != TK_EOF &&
         (!(this->token.getType() == TK_CURLY_BRACES &&
            this->token.getValue() == "}"))) {
    AST *node = this->statement();
    statements.push_back(node);
  }
  return new StatementListAST(statements);
}

AST *LangParser::statement() {
  AST *node;
  Token token = this->token;
  switch (token.getType()) {
  case TK_OUTPUTSTREAM:
    node = this->outputStream();
    this->consume(Token(TK_SEMICOLON, ""));
    return node;
  case TK_INPUTSTREAM:
    node = this->inputStream();
    this->consume(Token(TK_SEMICOLON, ""));
    return node;
  case TK_CURLY_BRACES:
    return this->block();
  case TK_RESERVED_WORD:
    if (token.getValue() == "if")
      return this->ifStatement();
    if (token.getValue() == "while")
      return this->whileStatement();
    if (token.getValue() == "for")
      return this->forStatement();
    if (token.getValue() == "return")
      return this->returnStatement();
    if (token.getValue() == "var") {
      node = this->variableDeclaration();
      this->consume(Token(TK_SEMICOLON, ""));
      return node;
    }
    if (token.getValue() == "func")
      return this->funcDeclaration();
  default:
    node = this->expression();
    this->consume(Token(TK_SEMICOLON, ""));
    return node;
  }
}

AST *LangParser::statementFunction() {
  std::vector<AST *> statements;
  while (this->token.getType() != TK_CURLY_BRACES &&
         this->token.getValue() != "}") {
    AST *node = this->statement();
    statements.push_back(node);
  }
  return new StatementListAST(statements);
}

AST *LangParser::block() {
  std::vector<AST *> statements;
  this->consume(Token(TK_CURLY_BRACES, "{"));

  while (this->token.getType() != TK_EOF &&
         (this->token.getType() != TK_CURLY_BRACES &&
          this->token.getValue() != "}")) {
    AST *node = this->statement();
    statements.push_back(node);
  }
  this->consume(Token(TK_CURLY_BRACES, "}"));

  return new BlockAST(statements);
}

AST *LangParser::returnStatement() {
  this->consume(Token(TK_RESERVED_WORD, "return"));
  /*AST *node = this->expression();*/
  this->consume(Token(TK_SEMICOLON, ""));
  return new ReturnAST();
}

