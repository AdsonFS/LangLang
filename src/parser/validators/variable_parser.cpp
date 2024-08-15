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
 
  if (this->token.getType() != TokenType::TK_ASSIGNMENT)
    throw std::runtime_error("Syntax error: expected ASSIGN");
  this->token = this->scanner.nextToken();

  AST* value = this->numericExpression();
   
  return new VariableDeclarationAST(type, identifier, value);
}
