#include "../lang_parser.h"

AST *LangParser::expression() {
  return this->assignment();
}

AST *LangParser::assignment() {
  AST *node = this->equalityExpression();
  if(this->token.getType() == TK_ASSIGNMENT) {
    IdentifierAST *identifierAST = dynamic_cast<IdentifierAST*>(node);
    if(identifierAST == nullptr) 
      throw SyntaxError(this->scanner.getLine(), this->token.getValue(),
                        this->scanner.getPosition(), "an identifier");
    this->consume(Token(TK_ASSIGNMENT, "")); 
    return new AssignmentVariableAST(identifierAST->token, this->expression());
  }
  while (this->isEqualityOperator()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->equalityExpression(), opToken);
  }
  return node;
}

AST *LangParser::equalityExpression() {
  AST *node = this->logicalExpression();
  while (this->isLogicalOperator()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->logicalExpression(), opToken);
  }
  return node;
}

AST *LangParser::logicalExpression() {
  AST *node = this->clause();
  while (this->isComparator()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    node = new BinaryOperatorAST(node, this->clause(), opToken);
  }
  return node;
}

AST *LangParser::clause() {
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

  if (this->token.getType() == TK_STRING) {
    Token token = this->token;
    this->token = this->scanner.nextToken();
    return new StringAST(token);
  }

  if (this->token.getType() == TK_IDENTIFIER) {
    Token token = this->token;
    this->token = this->scanner.nextToken();
    if (this->match(Token(TK_PARENTHESES, "("))) {
      std::vector<AST *> arguments;
      this->consume(Token(TK_PARENTHESES, "("));
      this->consume(Token(TK_PARENTHESES, ")"));
      return new CallAST(token, arguments);
    }
    return new IdentifierAST(token);
  }

  if (this->isPlusOrMinus()) {
    Token opToken = this->token;
    this->token = this->scanner.nextToken();
    return new UnaryOperatorAST(this->factor(), opToken);
  }

  if (this->token.getType() == TK_PARENTHESES &&
      this->token.getValue() == "(") {
    this->token = this->scanner.nextToken();
    AST *node = this->expression();
    if (this->token.getType() != TK_PARENTHESES ||
        this->token.getValue() != ")")
      throw SyntaxError(this->scanner.getLine(), this->token.getValue(),
                        this->scanner.getPosition(), ")");
    this->token = this->scanner.nextToken();
    return node;
  }
  throw SyntaxError(this->scanner.getLine(), this->token.getValue(),
                    this->scanner.getPosition(), "a factor");
}
