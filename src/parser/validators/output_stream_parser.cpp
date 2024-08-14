#include "../lang_parser.h"

AST* LangParser::outputStream() {
  std::vector<AST*> nodes;
  while(this->token.getType() == TK_OUTPUTSTREAM) {
    this->token = this->scanner.nextToken();
    if(this->token.getType() == TK_STRING) 
      nodes.push_back(this->stringExpression());
    else  
      nodes.push_back(this->numericExpression());
  }
  return new OutputStreamAST(nodes);
}
