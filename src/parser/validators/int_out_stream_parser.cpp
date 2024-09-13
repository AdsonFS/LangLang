#include "../lang_parser.h"

AST *LangParser::outputStream() {
  std::vector<AST *> nodes;
  while (this->token.getType() == TK_OUTPUTSTREAM) {
    this->token = this->scanner.nextToken();
    nodes.push_back(this->expression());
  }
  return new OutputStreamAST(nodes);
}

AST *LangParser::inputStream() {
  std::vector<IdentifierAST> identifiers;
  while (this->token.getType() == TK_INPUTSTREAM) {
    this->token = this->scanner.nextToken();

    Token identifier = this->consume(TokenType::TK_IDENTIFIER);
    identifiers.push_back(IdentifierAST(identifier));
  }
  return new InputStreamAST(identifiers);
}
