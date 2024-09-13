#include "interpreter_visitor.h"
#include "../core/lang_object.h"
#include "../error/error.h"
#include <iostream>
#include <ostream>

ScopedSymbolTable *InterpreterVisitor::scope;
std::unordered_map<AST *, int> InterpreterVisitor::jumpTable;

ASTValue *InterpreterVisitor::visitStatementList(StatementListAST *expr) {
  if (this->scope == nullptr)
    this->scope = new ScopedSymbolTable("global");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitBLock(BlockAST *expr) {
  this->scope = this->scope->newScope("block");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->scope = this->scope->previousScope;
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitReturn(ReturnAST *expr) {
  throw ReturnError(expr->value->accept(*this));
}

ASTValue *InterpreterVisitor::visitWhileStatement(WhileStatementAST *expr) {
  while (expr->condition->accept(*this)->isTrue()) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitForStatement(ForStatementAST *expr) {
  this->scope = this->scope->newScope("for");
  expr->initializer->accept(*this);
  while (expr->condition->accept(*this)->isTrue()) {
    expr->statements->accept(*this);
    expr->increment->accept(*this);
  }
  this->scope = this->scope->previousScope;
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitIfStatement(IfStatementAST *expr) {
  ASTValue *value = expr->condition->accept(*this);
  if (value->isTrue()) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  } else if (expr->elseStatements != nullptr) {
    this->scope = this->scope->newScope("else");
    expr->elseStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return new LangNil();
}

ASTValue *
InterpreterVisitor::visitFunctionDeclaration(FunctionDeclarationAST *expr) {

  ASTValue *type = nullptr;
  while (!expr->types.empty()) {
    ASTValue *t = this->scope->getSymbol(expr->types.top().getValue(), 0)->value;
    if (type == nullptr)
      type = t;
    else if (typeid(*t) != typeid(LangFunction))
      throw RuntimeError("type mismatch: " + expr->identifier.getValue());
    else
      type = new LangFunction(nullptr, type, this->scope);
    expr->types.pop();
  }

  FuncSymbol *func = new FuncSymbol(expr->identifier.getValue(),
                                    new LangFunction(expr->statements, type, this->scope));
  this->scope->set(func);
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitOutputStream(OutputStreamAST *expr) {
  int cnt = expr->outputs.size() - 1;
  for (auto &output : expr->outputs) {
    ASTValue *outputValue = output->accept(*this);
    std::cout << *outputValue << (cnt-- ? " " : "");
  }
  std::cout << std::endl;
  /*return (int)expr->outputs.size();*/
  return new LangNil();
}

ASTValue *InterpreterVisitor::visitInputStream(InputStreamAST *expr) {
  for (auto &identifier : expr->identifiers) {
    ASTValue *value = identifier.accept(*this);
    std::cin >> *value;
  }
  return new LangNil();
}

ASTValue *
InterpreterVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  ASTValue *value = expr->value->accept(*this);

  ASTValue *type = nullptr;
  while (!expr->types.empty()) {
    ASTValue *t = this->scope->getSymbol(expr->types.top().getValue(), 0)->value;
    if (type == nullptr)
      type = t;
    else if (typeid(*t) != typeid(LangFunction))
      throw RuntimeError("type mismatch: " + expr->identifier.getValue());
    else
      type = new LangFunction(nullptr, type, this->scope);
    expr->types.pop();
  }

  if (dynamic_cast<LangNil *>(value) != nullptr)
    value = new LangNil(type);

  this->scope->set(
      new VarSymbol(expr->identifier.getValue(), value));
  return new LangNil();
}

ASTValue *
InterpreterVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  ASTValue *value = expr->value->accept(*this);

  return this->scope->update(expr->identifier.getValue(), value, this->jumpTable[expr]);
}

ASTValue *InterpreterVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  // TODO
  if (expr->op.getType() == TK_LOGICAL_OPERATOR) {
    if (expr->op.getValue() == "&&")
      return new LangBoolean(expr->left->accept(*this)->isTrue() &&
                             expr->right->accept(*this)->isTrue());
    if (expr->op.getValue() == "||")
      return new LangBoolean(expr->left->accept(*this)->isTrue() ||
                             expr->right->accept(*this)->isTrue());
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }

  ASTValue *leftValue = expr->left->accept(*this);
  ASTValue *rightValue = expr->right->accept(*this);

  if (expr->op.getType() == TK_EQUALITY_OPERATOR) {
    if (expr->op.getValue() == "==")
      return *leftValue == *rightValue;
    if (expr->op.getValue() == "!=")
      return *leftValue != *rightValue;
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }

  switch (expr->op.getValue()[0]) {
  case '+':
    return *leftValue + *rightValue;
  case '-':
    return *leftValue - *rightValue;
  case '*':
    return *leftValue * *rightValue;
  case '/':
    return *leftValue / *rightValue;
  case '%':
    return *leftValue % *rightValue;

  case '<':
    return *leftValue < *rightValue;
  case '>':
    return *leftValue > *rightValue;
  default:
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }
  throw RuntimeError("invalid operator: " + expr->op.getValue());
}

ASTValue *InterpreterVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  switch (expr->op.getValue()[0]) {
  case '+':
    return expr->child->accept(*this);
  case '-':
    return -(*expr->child->accept(*this));
  }
  throw RuntimeError("invalid operator: " + expr->op.getValue());
}

ASTValue *InterpreterVisitor::visitCall(CallAST *expr) {
  // TODO
  ASTValue *value = this->scope->getValue(expr->identifier.getValue(), this->jumpTable[expr]);
  LangFunction *func = dynamic_cast<LangFunction *>(value);

  if (func == nullptr)
    throw RuntimeError("invalid function..: " + expr->identifier.getValue());

  ScopedSymbolTable *currentScope = this->scope;
  this->scope = func->getScope()->newScope("functioncall");
  ASTValue *returnValue = new LangVoid();
  try {
    func->getValue()->accept(*this);

  } catch (ReturnError &e) {
    returnValue = e.value;
  }
  this->scope = currentScope;

  /*if (!ScopedSymbolTable::isSameType(func->getReturnType(), returnValue))*/
    /*throw RuntimeError("invalid return type: " + expr->identifier.getValue());*/
  return returnValue;
}

ASTValue *InterpreterVisitor::visitIdentifier(IdentifierAST *expr) {
  return this->scope->getValue(expr->token.getValue(), this->jumpTable[expr]);
}

ASTValue *InterpreterVisitor::visitNumberExpr(NumberAST *expr) {
  return new LangNumber(std::stoi(expr->token.getValue()));
}

ASTValue *InterpreterVisitor::visitStringExpr(StringAST *expr) {
  return new LangString(expr->token.getValue());
}

ASTValue *InterpreterVisitor::visitVoid(VoidAST *expr) {
  return new LangVoid();
}
ASTValue *InterpreterVisitor::visitNil(NilAST *expr) { return new LangNil(); }
