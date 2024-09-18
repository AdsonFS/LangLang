#include "semantic_visitor.h"
#include "../core/lang_object.h"
#include "../error/error.h"
#include <iostream>

ScopedSymbolTable *SemanticVisitor::scope;
std::unordered_map<AST *, int> SemanticVisitor::jumpTable;
LangObject *SemanticVisitor::currentReturnType = nullptr;
LangObject *SemanticVisitor::currentFunctionType = nullptr;

ASTValue *SemanticVisitor::visitStatementList(StatementListAST *expr) {
  if (this->scope == nullptr)
    this->scope = new ScopedSymbolTable("global");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitBLock(BlockAST *expr) {
  this->scope = this->scope->newScope("block");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->scope = this->scope->previousScope;
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitReturn(ReturnAST *expr) {
  currentReturnType = expr->value->accept(*this)->value;
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitWhileStatement(WhileStatementAST *expr) {
  expr->condition->accept(*this);

  this->scope = this->scope->newScope("if");
  expr->ifStatements->accept(*this);
  this->scope = this->scope->previousScope;

  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitForStatement(ForStatementAST *expr) {
  this->scope = this->scope->newScope("for");
  expr->initializer->accept(*this);

  expr->condition->accept(*this);
  expr->statements->accept(*this);
  expr->increment->accept(*this);

  this->scope = this->scope->previousScope;
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitIfStatement(IfStatementAST *expr) {
  ASTValue *value = expr->condition->accept(*this);
  // if (value->isTrue()) {
  this->scope = this->scope->newScope("if");
  expr->ifStatements->accept(*this);
  this->scope = this->scope->previousScope;
  // } else if (expr->elseStatements != nullptr) {
  this->scope = this->scope->newScope("else");
  expr->elseStatements->accept(*this);
  this->scope = this->scope->previousScope;

  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitClassDeclaration(ClassDeclarationAST *expr) {
  ScopedSymbolTable *classScope = this->scope->newScope("class");
  ASTValue *superclass =
      expr->superclass == nullptr ? nullptr : expr->superclass->accept(*this);
  if (superclass != nullptr && typeid(*superclass->value) != typeid(LangClass))
    throw SemanticError("Invalid superclass for class " +
                        expr->identifier.getValue());
  if (superclass != nullptr) {
    LangClass *super = dynamic_cast<LangClass *>(superclass->value);
    classScope->setSymbols(super->getScope()->getSymbols());
  }

  ScopedSymbolTable *currentScope = this->scope;
  this->scope = classScope;

  for (auto &variable : expr->variables)
    variable->accept(*this);

  for (auto &method : expr->methods) {
    if (this->scope->check(method->identifier.getValue(), 0))
      this->scope->remove(method->identifier.getValue());
    method->accept(*this);
  }

  this->scope = currentScope;

  ClassSymbol *classSymbol = new ClassSymbol(
      expr->identifier.getValue(),
      new ASTValue(new LangClass(expr->identifier.getValue(), classScope)));
  if (!this->scope->set(classSymbol))
    throw SemanticError("Class " + expr->identifier.getValue() +
                        " already declared");

  return new ASTValue(new LangNil());
}

ASTValue *
SemanticVisitor::visitFunctionDeclaration(FunctionDeclarationAST *expr) {
  ASTValue *type = nullptr;
  std::stack<TypeAST *> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = types.top()->accept(*this);
    if (type == nullptr)
      type = t;
    else if (typeid(*t->value) != typeid(LangFunction))
      throw SemanticError("type mismatch: " + expr->identifier.getValue());
    else
      type = new ASTValue(new LangFunction(nullptr, type->value, this->scope));
    types.pop();
  }

  FuncSymbol *func =
      new FuncSymbol(expr->identifier.getValue(),
                     new ASTValue(new LangFunction(expr->statements,
                                                   type->value, this->scope)));
  if (!this->scope->set(func))
    throw SemanticError("Function " + expr->identifier.getValue() +
                        " already declared");

  ScopedSymbolTable *currentScope = this->scope;

  this->scope = currentScope->newScope("functionin");
  this->currentReturnType = new LangVoid();
  this->currentFunctionType = type->value;

  expr->statements->accept(*this);

  if (!ScopedSymbolTable::isSameType(type->value, this->currentReturnType))
    throw SemanticError("Invalid return type to function: " +
                        expr->identifier.getValue());

  this->scope = currentScope;
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitOutputStream(OutputStreamAST *expr) {
  for (auto &output : expr->outputs)
    output->accept(*this);
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitInputStream(InputStreamAST *expr) {
  for (auto &identifier : expr->identifiers)
    identifier.accept(*this);

  return new ASTValue(new LangNil());
}

ASTValue *
SemanticVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  ASTValue *value = expr->value->accept(*this);

  ASTValue *type = nullptr;
  std::stack<TypeAST *> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = types.top()->accept(*this);
    if (type == nullptr)
      type = t;
    else if (typeid(*t->value) != typeid(LangFunction))
      throw SemanticError("type mismatch: " + expr->identifier.getValue());
    else
      type = new ASTValue(new LangFunction(nullptr, type->value, this->scope));
    types.pop();
  }
  if (dynamic_cast<LangNil *>(value->value) != nullptr)
    value->value = new LangNil(type->value);
  else if (!ScopedSymbolTable::isSameType(type->value, value->value))
    throw SemanticError("Invalid type for variable " +
                        expr->identifier.getValue());
  if (!this->scope->set(new VarSymbol(expr->identifier.getValue(), value)))
    throw SemanticError("Variable " + expr->identifier.getValue() +
                        " already declared");
  return new ASTValue(new LangNil());
}

ASTValue *
SemanticVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  ASTValue *leftReference = expr->leftReference->accept(*this);
  ASTValue *value = expr->value->accept(*this);

  if (!ScopedSymbolTable::isSameType(leftReference->value, value->value))
    throw SemanticError("Type mismatch:: ");
  leftReference->value = value->value;
  return leftReference;
}

ASTValue *SemanticVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  // TODO
  ASTValue *leftValue = expr->left->accept(*this);
  ASTValue *rightValue = expr->right->accept(*this);

  if (!ScopedSymbolTable::isSameType(leftValue->value, rightValue->value))
    throw SemanticError("Invalid type for binary operator");

  return leftValue;
}

ASTValue *SemanticVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  // TODO
  expr->child->accept(*this);
  return new ASTValue(new LangNil());
}

ASTValue *SemanticVisitor::visitCall(CallAST *expr) {
  jumpTable[expr] =
      ScopedSymbolTable::jumpTo(expr->identifier.getValue(), this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Function " + expr->identifier.getValue() +
                        " not declared");
  LangObject *callee =
      this->scope->getSymbol(expr->identifier.getValue(), jumpTable[expr])
          ->value->value;
  if (typeid(*callee) == typeid(LangNil))
    callee = dynamic_cast<LangNil *>(callee)->getType();

  if (typeid(*callee) == typeid(LangFunction)) {
    LangFunction *function = dynamic_cast<LangFunction *>(callee);
    return new ASTValue(function->getReturnType());
  } else if (typeid(*callee) == typeid(LangClass)) {
    LangClass *lang_class = dynamic_cast<LangClass *>(callee);
    return new ASTValue(lang_class);
  }

  throw SemanticError("Invalid call to " + expr->identifier.getValue());
}

ASTValue *SemanticVisitor::visitPropertyChain(PropertyChainAST *expr) {
  ASTValue *value = expr->accesses[0]->accept(*this);

  for (int i = 1; i < expr->accesses.size(); i++) {
    LangClass *instance = dynamic_cast<LangClass *>(value->value);
    if (instance == nullptr)
      throw SemanticError("Invalid property chain");

    AST *node = expr->accesses[i];
    if (typeid(*node) == typeid(IdentifierAST)) {
      IdentifierAST *identifier = dynamic_cast<IdentifierAST *>(node);
      value = instance->getScope()->getValue(identifier->token.getValue(), 0);
    } else if (typeid(*node) == typeid(CallAST)) {
      CallAST *call = dynamic_cast<CallAST *>(node);
      value = instance->getScope()->getValue(call->identifier.getValue(), 0);
    }
  }
  return value;
}

ASTValue *SemanticVisitor::visitType(TypeAST *expr) {
  jumpTable[expr] =
      ScopedSymbolTable::jumpTo(expr->token.getValue(), this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Type " + expr->token.getValue() + " not declared");
  return this->scope->getSymbol(expr->token.getValue(), jumpTable[expr])->value;
}

ASTValue *SemanticVisitor::visitIdentifier(IdentifierAST *expr) {
  jumpTable[expr] =
      ScopedSymbolTable::jumpTo(expr->token.getValue(), this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Variable " + expr->token.getValue() + " not declared");
  return this->scope->getSymbol(expr->token.getValue(), jumpTable[expr])->value;
}

ASTValue *SemanticVisitor::visitNumberExpr(NumberAST *expr) {
  return new ASTValue(new LangNumber(0));
}

ASTValue *SemanticVisitor::visitStringExpr(StringAST *expr) {
  return new ASTValue(new LangString("..."));
}

ASTValue *SemanticVisitor::visitVoid(VoidAST *expr) {
  return new ASTValue(new LangVoid());
}
ASTValue *SemanticVisitor::visitNil(NilAST *expr) {
  return new ASTValue(new LangNil());
}
