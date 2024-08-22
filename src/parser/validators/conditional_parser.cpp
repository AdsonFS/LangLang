#include "../lang_parser.h"

AST *LangParser::ifStatement() {
  this->consume(Token(TK_RESERVED_WORD, "IF"));
  this->consume(Token(TK_PARENTHESES, "("));
  AST *conditional = this->expression();
  this->consume(Token(TK_PARENTHESES, ")"));
  this->consume(Token(TK_CURLY_BRACES, "{"));
  AST *statementList = this->statementList();
  this->consume(Token(TK_CURLY_BRACES, "}"));

  return new IfStatementAST(conditional,
                            dynamic_cast<StatementListAST *>(statementList));
}

/*AST *LangParser::conditional() {*/
/*  AST *left = this->expression();*/
/*  Token token = this->token;*/
/*  if (token.getType() == TK_COMPARATOR) {*/
/*    this->consume(Token(TK_COMPARATOR, ""));*/
/*    AST *right = this->expression();*/
/*    return new ConditionalAST(left, token, right);*/
/*  } else {*/
/*    throw std::runtime_error("Syntax error: expected COMPARATOR");*/
/*  }*/
/*}*/
