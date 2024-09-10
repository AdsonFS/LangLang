#include "../lang_parser.h"
#include <stack>
#include <string>

AST *LangParser::variableDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "var"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);

  std::stack<Token> types;
  while (this->match(TokenType::TK_ARROW)) {
    this->consume(Token(TokenType::TK_ARROW, "->"));
    types.push(this->consume(TokenType::TK_RESERVED_WORD));
  }
  if (types.empty())
    this->consume(Token(TokenType::TK_ARROW, "->"));

  AST *node;
  if (this->token.getType() != TK_SEMICOLON) {
    this->consume(Token(TokenType::TK_ASSIGNMENT, ":="));
    node = new VariableDeclarationAST(types, identifier, this->expression());
  } else
    node = new VariableDeclarationAST(types, identifier, new NilAST());
  return node;
}

AST *LangParser::funcDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "func"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);
  this->consume(Token(TokenType::TK_PARENTHESES, "("));
  this->consume(Token(TokenType::TK_PARENTHESES, ")"));
  /*this->consume(Token(TokenType::TK_ARROW, "->"));*/

  std::stack<Token> types;
  while (this->match(TokenType::TK_ARROW)) {
    this->consume(Token(TokenType::TK_ARROW, "->"));
    types.push(this->consume(TokenType::TK_RESERVED_WORD));
  }

  this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));
  AST *node = new FunctionDeclarationAST(
      identifier, types,
      dynamic_cast<StatementListAST *>(this->statementList()));
  this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
  return node;
}
