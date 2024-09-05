#include "../lang_parser.h"
#include <string>

AST *LangParser::variableDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "var"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);
  this->consume(Token(TokenType::TK_ARROW, "->"));
  Token type = this->consume(TokenType::TK_RESERVED_WORD);

  AST *node;
  if (type.getValue() == "number" || type.getValue() == "string") {
    if (this->token.getType() != TK_SEMICOLON) {
      this->consume(Token(TokenType::TK_ASSIGNMENT, ":="));
      node = new VariableDeclarationAST(type, identifier, this->expression());
    } else
      node = new VariableDeclarationAST(type, identifier, new NilAST());
    return node;
  }
  throw SyntaxError(this->scanner.getLine(), this->token.getValue(),
                    this->scanner.getPosition(), "NUMBER or STRING");
}

AST *LangParser::funcDeclaration() {
  this->consume(Token(TokenType::TK_RESERVED_WORD, "func"));
  Token identifier = this->consume(TokenType::TK_IDENTIFIER);
  this->consume(Token(TokenType::TK_PARENTHESES, "("));
  this->consume(Token(TokenType::TK_PARENTHESES, ")"));
  this->consume(Token(TokenType::TK_ARROW, "->"));

  std::string type = "func";
  if (!this->match(Token(TokenType::TK_CURLY_BRACES, "{")))
    type = this->consume(TokenType::TK_RESERVED_WORD).getValue();

  this->consume(Token(TokenType::TK_CURLY_BRACES, "{"));
  AST *node = new FunctionDeclarationAST(
      identifier, type,
      dynamic_cast<StatementListAST *>(this->statementList()));
  this->consume(Token(TokenType::TK_CURLY_BRACES, "}"));
  return node;
}
