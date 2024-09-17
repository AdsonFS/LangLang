#include "../lang_parser.h"
#include <stack>
#include <string>

AST *LangParser::variableDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "var"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);

  std::stack<Token> types;
  while (this->match(TokenType::TK_ARROW)) {
    this->consume(Token(TokenType::TK_ARROW, "->"));
    if (this->match(TokenType::TK_RESERVED_WORD))
      types.push(this->consume(TokenType::TK_RESERVED_WORD));
    else
      types.push(this->consume(TokenType::TK_IDENTIFIER));
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

AST *LangParser::classDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "class"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);
  this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));

  std::vector<FunctionDeclarationAST *> methods;
  std::vector<VariableDeclarationAST *> variables;
  while (1 < 2) {
    if (this->match(Token(TokenType::TK_RESERVED_WORD, "var"))) {
      variables.push_back(
          dynamic_cast<VariableDeclarationAST *>(this->variableDeclaration()));
      this->consume(TokenType::TK_SEMICOLON);
    } else if (this->match(Token(TokenType::TK_RESERVED_WORD, "func")))
      methods.push_back(
          dynamic_cast<FunctionDeclarationAST *>(this->funcDeclaration()));
    else
      break;
  }
  this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
  return new ClassDeclarationAST(identifier, variables, methods);
}
