#include "../lang_parser.h"
#include <string>

AST *LangParser::variableDeclaration() {
  Token type = this->consume(Token(TokenType::TK_RESERVED_WORD, ""));
  Token identifier = this->consume(Token(TokenType::TK_IDENTIFIER, ""));
  

  if (type.getValue() == "func") {
    this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));
    return new FunctionAST(identifier, dynamic_cast<StatementListAST*>(this->statementList()));
  }

  this->consume(Token(TokenType::TK_ASSIGNMENT, ""));

  if (type.getValue() == "number" || type.getValue() == "string")
    return new VariableDeclarationAST(type, identifier,
                                      this->expression());
  throw std::runtime_error("Syntax error: expected NUMBER or STRING");
}
