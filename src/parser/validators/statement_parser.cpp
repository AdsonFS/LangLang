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
    } else if (this->token.getType() == TK_PARENTHESES &&
               this->token.getValue() == "(") { // function call
      node = new IdentifierAST(token);
      this->consume(Token(TK_PARENTHESES, "("));
      this->consume(Token(TK_PARENTHESES, ")"));

      this->consume(Token(TK_SEMICOLON, ""));
      return node;
    } else {
      throw std::runtime_error(
          "Syntax error: expected ASSIGNMENT or SEMICOLON");
    }
  case TK_RESERVED_WORD:
    if (token.getValue() == "IF") 
      return this->ifStatement();
    if (token.getValue() == "WHILE")
        return this->whileStatement();
    node = this->variableDeclaration();
    if (token.getValue() == "FUNC")
      this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));

    else
      this->consume(Token(TK_SEMICOLON, ""));
    return node;
  default:
    throw std::runtime_error("Syntax error: found " + token.getValue());
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
