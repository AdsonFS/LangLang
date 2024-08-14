#ifndef AST_H
#define AST_H

#include "../tokens/token.h"
#include <string>
#include <variant>
#include <vector>

class AST {
public:
  virtual std::variant<int, std::string> solve();
};

class OutputStreamAST : public AST {
public:
  OutputStreamAST(std::vector<AST*> outputs) : outputs(outputs) {}
  std::variant<int, std::string> solve() override;
private:
  std::vector<AST*> outputs;
};

class BinaryOperatorAST : public AST {
public:
  BinaryOperatorAST(AST *left, AST *right, Token op)
      : left(left), right(right), op(op) {}
  std::variant<int, std::string> solve() override;
private:
  AST *left;
  AST *right;
  Token op;
};

class UnaryOperatorAST : public AST {
public:
  UnaryOperatorAST(AST* child, Token op) : child(child), op(op) {}
  std::variant<int, std::string> solve() override;
private:
  AST* child;
  Token op;
};

class NumberAST : public AST {
public:
  NumberAST(Token token) : token(token) {}
  std::variant<int, std::string> solve() override;
private:
  Token token;
};

class StringAST : public AST {
public:
  StringAST(Token token) : token(token) {}
  std::variant<int, std::string> solve() override;
private:
  Token token;
};





















#endif // !AST_H
