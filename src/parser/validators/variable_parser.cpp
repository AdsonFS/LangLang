#include "../lang_parser.h"

AST* LangParser::variableDeclaration() {
  if (this->token.getType() != TokenType::TK_RESERVED_WORD)
    throw std::runtime_error("Syntax error: expected VAR");
  if (this->token.getValue() != "NUMBER")
    throw std::runtime_error("Syntax error: expected NUMBER");
  Token type = this->token; 

  this->token = this->scanner.nextToken();
  if (this->token.getType() != TokenType::TK_IDENTIFIER)
    throw std::runtime_error("Syntax error: expected IDENTIFIER");
  Token identifier = this->token;
  this->token = this->scanner.nextToken();

  return new VariableDeclarationAST(type, identifier);
}
