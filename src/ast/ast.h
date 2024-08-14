#ifndef AST_H
#define AST_H

#include "../tokens/token.h"
#include <string>
#include <variant>
#include <vector>

typedef std::variant<int, std::string> ASTValue;

class AST {
public:
  virtual ASTValue solve();
};

class StatementListAST : public AST {
public:
  StatementListAST(std::vector<AST*> statements) : statements(statements) {}
  ASTValue solve() override;
private:
  std::vector<AST*> statements;
};

class OutputStreamAST : public AST {
public:
  OutputStreamAST(std::vector<AST*> outputs) : outputs(outputs) {}
  ASTValue solve() override;
private:
  std::vector<AST*> outputs;
};

class BinaryOperatorAST : public AST {
public:
  BinaryOperatorAST(AST *left, AST *right, Token op)
      : left(left), right(right), op(op) {}
  ASTValue solve() override;
private:
  AST *left;
  AST *right;
  Token op;
};

class UnaryOperatorAST : public AST {
public:
  UnaryOperatorAST(AST* child, Token op) : child(child), op(op) {}
  ASTValue solve() override;
private:
  AST* child;
  Token op;
};

class NumberAST : public AST {
public:
  NumberAST(Token token) : token(token) {}
  ASTValue solve() override;
private:
  Token token;
};

class StringAST : public AST {
public:
  StringAST(Token token) : token(token) {}
  ASTValue solve() override;
private:
  Token token;
};





















#endif // !AST_H
