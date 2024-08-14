#include "expression_parser.h"

ExpressionParser::ExpressionParser(LexiScanner &scanner, Token &token)
    : scanner(scanner), token(token) {}

/*
 * expression: term ((+|-) term)*
 * term: factor ((*|/|%) factor)*
 * factor: (+\-)factor | number | '(' expression ')'
 */

AST* ExpressionParser::parser() { return this->expression(); }

AST* ExpressionParser::expression() {
  AST* node = this->term();
  while (this->isPlusOrMinus()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->term(), opToken);
  }
  return node;
}

AST* ExpressionParser::term() {
  AST* node = this->factor();
  while ( this->isMultOrDivOrMod()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->factor(), opToken);
  }
  return node;
}

AST* ExpressionParser::factor() {
  if (this->token.getType() == TK_NUMBER) {
    Token token = this->token;
    this->token = this->scanner.nextToken();
    return new NumberAST(token);
  }

  if (this->isPlusOrMinus()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    return new UnaryOperatorAST(this->factor(), opToken);
  }


  if (this->token.getType() == TK_PARENTHESES && this->token.getValue() == "(") {
    this->token = this->scanner.nextToken();
    AST* node = this->expression();
    if (this->token.getType() != TK_PARENTHESES || this->token.getValue() != ")")
      throw std::runtime_error("Expected ')' but got: " + this->token.getValue());
    this->token = this->scanner.nextToken();
    return node;
  }
  throw std::runtime_error("Invalid expression");
}

bool ExpressionParser::isPlusOrMinus() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "+" || this->token.getValue() == "-");
}

bool ExpressionParser::isMultOrDivOrMod() {
  return this->token.getType() == TK_OPERATOR &&
         (this->token.getValue() == "*" || this->token.getValue() == "/" ||
          this->token.getValue() == "%");
}
