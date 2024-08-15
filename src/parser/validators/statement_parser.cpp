#include "../lang_parser.h"

AST *LangParser::statementList() {
  std::vector<AST *> statements;
  while (this->token.getType() != TK_EOF) {
    AST *node = this->statement();
    statements.push_back(node);
  }
  return new StatementListAST(statements);
}

AST *LangParser::statement() {
  AST *node;
  switch (this->token.getType()) {
  case TK_OUTPUTSTREAM:
    node = this->outputStream();
    if (this->token.getType() != TK_SEMICOLON)
      throw std::runtime_error("Syntax error: expected SEMICOLON");
    this->token = this->scanner.nextToken();
    return node;
  case TK_RESERVED_WORD:
    node = this->variableDeclaration();
    if (this->token.getType() != TK_SEMICOLON)
      throw std::runtime_error("Syntax error: expected SEMICOLON");
    this->token = this->scanner.nextToken();
    return node;
  default:
    throw std::runtime_error("Syntax error: expected OUTPUTSTREAM");
  }
}
