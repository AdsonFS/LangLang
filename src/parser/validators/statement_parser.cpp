#include "../lang_parser.h"

AST *LangParser::statementList() {
  std::vector<AST *> statements;
  while (this->token.getType() != TK_EOF &&
         (this->token.getType() != TK_CURLY_BRACES &&
          this->token.getValue() != "}")) {
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
  case TK_IDENTIFIER:
    this->consume(Token(TK_IDENTIFIER, ""));
    if (this->token.getType() == TK_ASSIGNMENT) {
        this->consume(Token(TK_ASSIGNMENT, ""));
        node = new AssignmentVariableAST(token, this->expression());
        this->consume(Token(TK_SEMICOLON, ""));
        return node;
    } else { // function call
      node = new IdentifierAST(token);
      this->consume(Token(TK_SEMICOLON, ""));
      return node;
    }
  case TK_RESERVED_WORD:
    node = this->variableDeclaration();
    if (token.getValue() == "FUNC")
      this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
    else
      this->consume(Token(TK_SEMICOLON, ""));
    return node;
  default:
    throw std::runtime_error("Syntax error: expected OUTPUTSTREAM");
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


