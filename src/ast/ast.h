#ifndef AST_H
#define AST_H

#include "../tokens/token.h"
#include <charconv>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

typedef std::variant<int, std::string> ASTValue;
class AST {
public:
  virtual ASTValue solve();

protected:
  static std::unordered_map<std::string, ASTValue>
      hashTable;
};

class StatementListAST : public AST {
public:
  StatementListAST(std::vector<AST *> statements) : statements(statements) {}
  ASTValue solve() override;

private:
  std::vector<AST *> statements;
};

class VariableDeclarationAST : public AST {
public:
  VariableDeclarationAST(Token type, Token identifier, AST* value)
      : type(type), identifier(identifier), value(value) {}
  ASTValue solve() override;

private:
  Token type;
  Token identifier;
  AST* value;
};

class OutputStreamAST : public AST {
public:
  OutputStreamAST(std::vector<AST *> outputs) : outputs(outputs) {}
  ASTValue solve() override;

private:
  std::vector<AST *> outputs;
};


class InputStreamAST : public AST {
public:
  InputStreamAST(std::vector<Token> identifiers) : identifiers(identifiers) {}
  ASTValue solve() override;

private:
  std::vector<Token> identifiers;
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
  UnaryOperatorAST(AST *child, Token op) : child(child), op(op) {}
  ASTValue solve() override;

private:
  AST *child;
  Token op;
};

class NumberAST : public AST {
public:
  NumberAST(Token token) : token(token) {}
  ASTValue solve() override;

private:
  Token token;
};

class IdentifierAST : public AST {
public:
  IdentifierAST(Token token) : token(token) {}
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
