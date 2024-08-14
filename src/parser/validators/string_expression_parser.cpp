#include "../lang_parser.h"

AST* LangParser::stringExpression() {
  AST* node = this->string();
  while (this->isPlusOrMinus()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->stringExpression(), opToken);
  }
  return node;
}

AST* LangParser::string() {
  if (this->token.getType() != TK_STRING)
    throw std::runtime_error("Expected string but got: " + this->token.getValue());
  Token token = this->token;
  this->token = this->scanner.nextToken();
  return new StringAST(token);
}
