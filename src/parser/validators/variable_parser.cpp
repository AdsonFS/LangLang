#include "../lang_parser.h"
#include <string>

AST *LangParser::variableDeclaration() {
  Token type = this->consume(Token(TokenType::TK_RESERVED_WORD, ""));
  Token identifier = this->consume(Token(TokenType::TK_IDENTIFIER, ""));
  this->consume(Token(TokenType::TK_ASSIGNMENT, ""));

  if (type.getValue() == "NUMBER")
    return new VariableDeclarationAST(type, identifier,
                                      this->numericExpression());
  if (type.getValue() == "STRING")
    return new VariableDeclarationAST(type, identifier,
                                      this->stringExpression());
  throw std::runtime_error("Syntax error: expected NUMBER or STRING");
}
