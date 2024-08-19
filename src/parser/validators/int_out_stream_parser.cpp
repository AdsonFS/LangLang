#include "../lang_parser.h"

AST *LangParser::outputStream() {
  std::vector<AST *> nodes;
  while (this->token.getType() == TK_OUTPUTSTREAM) {
    this->token = this->scanner.nextToken();

    if (this->token.getType() == TK_STRING)
      nodes.push_back(this->stringExpression());
    else
      nodes.push_back(this->numericExpression());
  }
  return new OutputStreamAST(nodes);
}

AST *LangParser::inputStream() {
  std::vector<Token> identifiers;
  while (this->token.getType() == TK_INPUTSTREAM) {
    this->token = this->scanner.nextToken();

    Token identifier = this->consume(Token(TokenType::TK_IDENTIFIER, ""));
    identifiers.push_back(identifier);
  }
  return new InputStreamAST(identifiers);
}
