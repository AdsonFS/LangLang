#ifndef AST_H
#define AST_H

#include "../core/core.h"
#include "../symbols/symbol.h"
#include "../tokens/token.h"
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class AST {
public:
  virtual ASTValue solve();

protected:
  static ScopedSymbolTable *scope;
};

class StatementListAST : public AST {
public:
  StatementListAST(std::vector<AST *> statements) : statements(statements) {}
  ASTValue solve() override;

private:
  std::vector<AST *> statements;
};

class WhileStatementAST : public AST {
public:
  WhileStatementAST(AST *condition, StatementListAST *ifStatements)
      : condition(condition), ifStatements(ifStatements) {}
  ASTValue solve() override;

private:
  AST *condition;
  StatementListAST *ifStatements;
};

class IfStatementAST : public AST {
public:
  IfStatementAST(AST *condition, StatementListAST *ifStatements)
      : condition(condition), ifStatements(ifStatements) {}
  ASTValue solve() override;

private:
  AST *condition;
  StatementListAST *ifStatements;
};

class FunctionAST : public AST {
public:
  FunctionAST(Token identifier, StatementListAST *statements)
      : identifier(identifier), statements(statements) {}
  ASTValue solve() override;

private:
  Token identifier;
  StatementListAST *statements;
};

class AssignmentVariableAST : public AST {
public:
  AssignmentVariableAST(Token identifier, AST *value)
      : identifier(identifier), value(value) {}
  ASTValue solve() override;

private:
  Token identifier;
  AST *value;
};

class VariableDeclarationAST : public AST {
public:
  VariableDeclarationAST(Token type, Token identifier, AST *value)
      : type(type), identifier(identifier), value(value) {}
  ASTValue solve() override;

private:
  Token type;
  Token identifier;
  AST *value;
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
  BinaryOperatorAST(AST *left, AST *right, Token op,
                    bool isParenthesized = false)
      : left(left), right(right), op(op), isParenthesized(isParenthesized) {}
  ASTValue solve() override;

private:
  AST *left;
  AST *right;
  Token op;
  bool isParenthesized;
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
