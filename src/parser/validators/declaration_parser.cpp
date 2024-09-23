#include "../lang_parser.h"
#include <stack>
#include <string>

AST *LangParser::variableDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "var"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);
  TypeAST *type = dynamic_cast<TypeAST *>(this->type());

  AST *node;
  if (this->token.getType() != TK_SEMICOLON) {
    this->consume(Token(TokenType::TK_ASSIGNMENT, ":="));
    node = new VariableDeclarationAST(type, identifier, this->expression());
  } else
    node = new VariableDeclarationAST(type, identifier, new NilAST());
  return node;
}

AST *LangParser::funcDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "func"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);

  this->consume(Token(TokenType::TK_PARENTHESES, "("));
  std::vector<VariableDeclarationAST *> parameters;
  while (this->match(TokenType::TK_IDENTIFIER)) {
    Token identifier = this->consume(TokenType::TK_IDENTIFIER);
    TypeAST *type = dynamic_cast<TypeAST *>(this->type());
    parameters.push_back(
        new VariableDeclarationAST(type, identifier, new NilAST()));
    if (this->match(TokenType::TK_COMMA))
      this->consume(TokenType::TK_COMMA);
  }
  this->consume(Token(TokenType::TK_PARENTHESES, ")"));


  TypeAST *type = dynamic_cast<TypeAST *>(this->type());
  this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));
  AST *node = new FunctionDeclarationAST(
      identifier, type, parameters,
      dynamic_cast<StatementListAST *>(this->statementList()));
  this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
  return node;
}

AST *LangParser::classDeclaration() {
  TypeAST *superclass = nullptr;
  this->consume(Token(TokenType::TK_RESERVED_WORD, "class"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);

  if (this->match(TK_COLON)) {
    std::stack<IdentifierAST *> types;
    this->consume(TK_COLON);
    if (this->match(TK_RESERVED_WORD))
      types.push(new IdentifierAST(this->consume(TK_RESERVED_WORD)));
    else
      types.push(new IdentifierAST(this->consume(TK_IDENTIFIER)));
    superclass = new TypeAST(types);
  }

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
  return new ClassDeclarationAST(identifier, superclass, variables, methods);
}

AST *LangParser::type() {
  std::stack<IdentifierAST *> types;
  while (this->match(TokenType::TK_ARROW)) {
    this->consume(Token(TokenType::TK_ARROW, "->"));
    if (this->match(TokenType::TK_RESERVED_WORD))
      types.push(new IdentifierAST(this->consume(TokenType::TK_RESERVED_WORD)));
    else
      types.push(new IdentifierAST(this->consume(TokenType::TK_IDENTIFIER)));
  }
  if (types.empty())
    this->consume(Token(TokenType::TK_ARROW, "->"));
  return new TypeAST(types);
}
