#include "../lang_parser.h"

AST *LangParser::numericExpression() {
  AST *node = this->term();
  while (this->isPlusOrMinus()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->term(), opToken);
  }
  return node;
}

AST *LangParser::term() {
  AST *node = this->factor();
  while (this->isMultOrDivOrMod()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->factor(), opToken);
  }
  return node;
}

AST *LangParser::factor() {
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

  if (this->token.getType() == TK_PARENTHESES &&
      this->token.getValue() == "(") {
    this->token = this->scanner.nextToken();
    AST *node = this->numericExpression();
    if (this->token.getType() != TK_PARENTHESES ||
        this->token.getValue() != ")")
      throw std::runtime_error("Expected ')' but got: " +
                               this->token.getValue());
    this->token = this->scanner.nextToken();
    return node;
  }
  throw std::runtime_error("Invalid expression");
}
