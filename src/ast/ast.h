#ifndef AST_H
#define AST_H

#include "../core/core.h"
#include "../symbols/symbol.h"
#include "../tokens/token.h"
#include "visitor.h"
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class AST {
public:
  virtual ASTValue *accept(ASTVisitor &visitor);
};

class StatementListAST : public AST {
public:
  StatementListAST(std::vector<AST *> statements) : statements(statements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::vector<AST *> statements;
};

class WhileStatementAST : public AST {
public:
  WhileStatementAST(AST *condition, StatementListAST *ifStatements)
      : condition(condition), ifStatements(ifStatements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  AST *condition;
  StatementListAST *ifStatements;
};

class ForStatementAST : public AST {
public:
  ForStatementAST(AST *initializer, AST *condition, AST *increment,
                  StatementListAST *ifStatements)
      : initializer(initializer), condition(condition), increment(increment),
        statements(ifStatements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  AST *initializer;
  AST *condition;
  AST *increment;
  StatementListAST *statements;
};

class BlockAST : public AST {
public:
  BlockAST(std::vector<AST *> statements) : statements(statements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::vector<AST *> statements;
};

class ReturnAST : public AST {
public:
  ReturnAST(AST *value) : value(value) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  AST *value;
};

class IfStatementAST : public AST {
public:
  IfStatementAST(AST *condition, StatementListAST *ifStatements,
                 StatementListAST *elseStatements)
      : condition(condition), ifStatements(ifStatements),
        elseStatements(elseStatements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  AST *condition;
  StatementListAST *ifStatements;
  StatementListAST *elseStatements;
};

class TypeAST;
class ClassDeclarationAST : public AST {
public:
  ClassDeclarationAST(Token identifier, TypeAST *superclass,
                      std::vector<VariableDeclarationAST *> variables,
                      std::vector<FunctionDeclarationAST *> methods)
      : identifier(identifier), superclass(superclass), variables(variables), methods(methods) {}

  ASTValue *accept(ASTVisitor &visitor) override;
  Token identifier;
  TypeAST *superclass;
  std::vector<VariableDeclarationAST *> variables;
  std::vector<FunctionDeclarationAST *> methods;
};

class FunctionDeclarationAST : public AST {
public:
  FunctionDeclarationAST(Token identifier, std::stack<TypeAST*> types,
                         StatementListAST *statements)
      : identifier(identifier), types(types), statements(statements) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::stack<TypeAST*> types;
  Token identifier;
  StatementListAST *statements;
};

class OutputStreamAST : public AST {
public:
  OutputStreamAST(std::vector<AST *> outputs) : outputs(outputs) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::vector<AST *> outputs;
};

class InputStreamAST : public AST {
public:
  InputStreamAST(std::vector<IdentifierAST> identifiers)
      : identifiers(identifiers) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::vector<IdentifierAST> identifiers;
};

class VariableDeclarationAST : public AST {
public:
  VariableDeclarationAST(std::stack<TypeAST*> types, Token identifier, AST *value)
      : types(types), identifier(identifier), value(value) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::stack<TypeAST*> types;
  Token identifier;
  AST *value;
};

class AssignmentVariableAST : public AST {
public:
  AssignmentVariableAST(AST *leftReference, Token assignmentOperator, AST *value)
      : leftReference(leftReference), assignmentOperator(assignmentOperator), value(value) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token assignmentOperator;
  AST *leftReference;
  AST *value;
};

class BinaryOperatorAST : public AST {
public:
  BinaryOperatorAST(AST *left, AST *right, Token op)
      : left(left), right(right), op(op) {}
  ASTValue *accept(ASTVisitor &visitor) override;
  AST *left;
  AST *right;
  Token op;
};

class UnaryOperatorAST : public AST {
public:
  UnaryOperatorAST(AST *child, Token op) : child(child), op(op) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  AST *child;
  Token op;
};

class CallAST : public AST {
public:
  CallAST(Token identifier, std::vector<AST *> arguments)
      : identifier(identifier), arguments(arguments) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token identifier;
  std::vector<AST *> arguments;
};

class PropertyChainAST : public AST {
public:
  PropertyChainAST(std::vector<AST *> accesses) : accesses(accesses) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  std::vector<AST *> accesses;
};

class TypeAST : public AST {
public:
  TypeAST(Token token) : token(token) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token token;
};

class IdentifierAST : public AST {
public:
  IdentifierAST(Token token) : token(token) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token token;
};

class NumberAST : public AST {
public:
  NumberAST(Token token) : token(token) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token token;
};

class StringAST : public AST {
public:
  StringAST(Token token) : token(token) {}
  ASTValue *accept(ASTVisitor &visitor) override;

  Token token;
};

class VoidAST : public AST {
public:
  VoidAST(){};
  ASTValue *accept(ASTVisitor &visitor) override;
};

class NilAST : public AST {
public:
  NilAST(){};
  ASTValue *accept(ASTVisitor &visitor) override;
};

#endif // !AST_H
