#include "../lang_parser.h"
#include <string>

AST *LangParser::variableDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "var"));
  Token identifier = this->consume(Token(TokenType::TK_IDENTIFIER, ""));
  this->consume(Token(TokenType::TK_ARROW, "->"));
  
  AST *node;
  if (this->token.getValue() == "number" || this->token.getValue() == "string") {
    Token type = this->consume(Token(TokenType::TK_RESERVED_WORD, ""));
    this->consume(Token(TokenType::TK_ASSIGNMENT, ":="));
    node = new VariableDeclarationAST(type, identifier, this->expression());
    this->consume(Token(TokenType::TK_SEMICOLON, ""));
    return node;
  }
  if (this->token.getType() == TokenType::TK_PARENTHESES) {
    this->consume(Token(TokenType::TK_PARENTHESES, "("));
    this->consume(Token(TokenType::TK_PARENTHESES, ")"));
    this->consume(Token(TokenType::TK_ASSIGNMENT, ":="));
    this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));
    node = new FunctionAST(
        identifier, dynamic_cast<StatementListAST *>(this->statementList()));
    this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
    return node;
  }
  throw std::runtime_error(
      "Syntax error: expected NUMBER or STRING ou FUNCTION");
}
