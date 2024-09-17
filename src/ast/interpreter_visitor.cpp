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
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitBLock(BlockAST *expr) {
  this->scope = this->scope->newScope("block");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->scope = this->scope->previousScope;
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitReturn(ReturnAST *expr) {
  throw ReturnError(expr->value->accept(*this));
}

ASTValue *InterpreterVisitor::visitWhileStatement(WhileStatementAST *expr) {
  while (expr->condition->accept(*this)->value->isTrue()) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitForStatement(ForStatementAST *expr) {
  this->scope = this->scope->newScope("for");
  expr->initializer->accept(*this);
  while (expr->condition->accept(*this)->value->isTrue()) {
    expr->statements->accept(*this);
    expr->increment->accept(*this);
  }
  this->scope = this->scope->previousScope;
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitIfStatement(IfStatementAST *expr) {
  ASTValue *value = expr->condition->accept(*this);
  if (value->value->isTrue()) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  } else if (expr->elseStatements != nullptr) {
    this->scope = this->scope->newScope("else");
    expr->elseStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitClassDeclaration(ClassDeclarationAST *expr) {
  ScopedSymbolTable *currentScope = this->scope;
  ScopedSymbolTable *classScope =
      this->scope->newScope(expr->identifier.getValue());
  this->scope = classScope;

  for (auto &variables : expr->variables)
    variables->accept(*this);

  for (auto &methods : expr->methods)
    methods->accept(*this);

  this->scope = currentScope;

  ClassSymbol *classSymbol = new ClassSymbol(
      expr->identifier.getValue(),
      new ASTValue(new LangClass(expr->identifier.getValue(), classScope)));
  this->scope->set(classSymbol);
  return new ASTValue(new LangNil());
}

ASTValue *
InterpreterVisitor::visitFunctionDeclaration(FunctionDeclarationAST *expr) {

  ASTValue *type = nullptr;
  std::stack<Token> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = this->scope->getSymbol(types.top().getValue(), 0)->value;
    if (type == nullptr)
      type = t;
    else
      type = new ASTValue(new LangFunction(nullptr, type->value, this->scope));
    types.pop();
  }

  FuncSymbol *func =
      new FuncSymbol(expr->identifier.getValue(),
                     new ASTValue(new LangFunction(expr->statements,
                                                   type->value, this->scope)));
  this->scope->set(func);
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitOutputStream(OutputStreamAST *expr) {
  int cnt = expr->outputs.size() - 1;
  for (auto &output : expr->outputs) {
    LangObject *outputValue = output->accept(*this)->value;
    std::cout << *outputValue << (cnt-- ? " " : "");
  }
  std::cout << std::endl;
  return new ASTValue(new LangNil());
}

ASTValue *InterpreterVisitor::visitInputStream(InputStreamAST *expr) {
  for (auto &identifier : expr->identifiers) {
    LangObject *value = identifier.accept(*this)->value;
    std::cin >> *value;
  }
  return new ASTValue(new LangNil());
}

ASTValue *
InterpreterVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  ASTValue *value = expr->value->accept(*this);

  ASTValue *type = nullptr;
  std::stack<TypeAST*> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = types.top()->accept(*this);
    if (type == nullptr)
      type = t;
    else
      type = new ASTValue(new LangFunction(nullptr, type->value, this->scope));
    types.pop();
  }
  if (dynamic_cast<LangNil *>(value->value) != nullptr)
    value->value = new LangNil(type->value);
  this->scope->set(new VarSymbol(expr->identifier.getValue(), value));
  return new ASTValue(new LangNil());
}

ASTValue *
InterpreterVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  ASTValue *leftReference = expr->leftReference->accept(*this);
  ASTValue *value = expr->value->accept(*this);
  leftReference->value = value->value;
  return leftReference;
}

ASTValue *InterpreterVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  if (expr->op.getType() == TK_LOGICAL_OPERATOR) {
    if (expr->op.getValue() == "&&")
      return new ASTValue(
          new LangBoolean(expr->left->accept(*this)->value->isTrue() &&
                          expr->right->accept(*this)->value->isTrue()));
    if (expr->op.getValue() == "||")
      return new ASTValue(
          new LangBoolean(expr->left->accept(*this)->value->isTrue() ||
                          expr->right->accept(*this)->value->isTrue()));
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }

  ASTValue *leftValue = expr->left->accept(*this);
  ASTValue *rightValue = expr->right->accept(*this);

  if (expr->op.getType() == TK_EQUALITY_OPERATOR) {
    if (expr->op.getValue() == "==")
      return new ASTValue(*leftValue->value == *rightValue->value);
    if (expr->op.getValue() == "!=")
      return new ASTValue(*leftValue->value != *rightValue->value);
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }

  switch (expr->op.getValue()[0]) {
  case '+':
    return new ASTValue(*leftValue->value + *rightValue->value);
  case '-':
    return new ASTValue(*leftValue->value - *rightValue->value);
  case '*':
    return new ASTValue(*leftValue->value * *rightValue->value);
  case '/':
    return new ASTValue(*leftValue->value / *rightValue->value);
  case '%':
    return new ASTValue(*leftValue->value % *rightValue->value);

  case '<':
    return new ASTValue(*leftValue->value < *rightValue->value);
  case '>':
    return new ASTValue(*leftValue->value > *rightValue->value);
  default:
    throw RuntimeError("invalid operator: " + expr->op.getValue());
  }
  throw RuntimeError("invalid operator: " + expr->op.getValue());
}

ASTValue *InterpreterVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  // TODO
  switch (expr->op.getValue()[0]) {
  case '+':
    return expr->child->accept(*this);
  case '-':
    return new ASTValue(-(*expr->child->accept(*this)->value));
  }
  throw RuntimeError("invalid operator: " + expr->op.getValue());
}

ASTValue *InterpreterVisitor::visitCall(LangObject *callee, std::string name) {
  if (typeid(*callee) == typeid(LangFunction)) {
    LangFunction *func = dynamic_cast<LangFunction *>(callee);
    ScopedSymbolTable *currentScope = this->scope;
    this->scope = func->getScope()->newScope("functioncall");
    LangObject *returnValue = new LangVoid();
    try {
      func->getValue()->accept(*this);

    } catch (ReturnError &e) {
      returnValue = e.value->value;
    }
    this->scope = currentScope;
    return new ASTValue(returnValue);
  } else if (typeid(*callee) == typeid(LangClass)) {
    LangClass *lang_class = dynamic_cast<LangClass *>(callee);
    return new ASTValue(lang_class);
  }
  throw RuntimeError("invalid call to " + name);
}

ASTValue *InterpreterVisitor::visitCall(CallAST *expr) {
  ASTValue *value =
      this->scope->getValue(expr->identifier.getValue(), this->jumpTable[expr]);
  return this->visitCall(value->value, expr->identifier.getValue());
}

ASTValue *InterpreterVisitor::visitPropertyChain(PropertyChainAST *expr) {
  ASTValue *value = expr->accesses[0]->accept(*this);

  for (int i = 1; i < expr->accesses.size(); i++) {
    LangClass *instance = dynamic_cast<LangClass *>(value->value);

    AST *node = expr->accesses[i];
    if (typeid(*node) == typeid(IdentifierAST)) {
      IdentifierAST *identifier = dynamic_cast<IdentifierAST *>(node);
      value = instance->getScope()->getValue(identifier->token.getValue(),
    0);
    } else if (typeid(*node) == typeid(CallAST)) {
      CallAST *call = dynamic_cast<CallAST *>(node);
      value = instance->getScope()->getValue(call->identifier.getValue(),
    0);
      value = this->visitCall(value->value, call->identifier.getValue());
    }
  }

  return value;
  throw RuntimeError("invalid property chain");
}

ASTValue *InterpreterVisitor::visitType(TypeAST *expr) {
  return this->scope->getValue(expr->token.getValue(), this->jumpTable[expr]);
}

ASTValue *InterpreterVisitor::visitIdentifier(IdentifierAST *expr) {
  return this->scope->getValue(expr->token.getValue(), this->jumpTable[expr]);
}

ASTValue *InterpreterVisitor::visitNumberExpr(NumberAST *expr) {
  return new ASTValue(new LangNumber(std::stoi(expr->token.getValue())));
}

ASTValue *InterpreterVisitor::visitStringExpr(StringAST *expr) {
  return new ASTValue(new LangString(expr->token.getValue()));
}

ASTValue *InterpreterVisitor::visitVoid(VoidAST *expr) {
  return new ASTValue(new LangVoid());
}
ASTValue *InterpreterVisitor::visitNil(NilAST *expr) {
  return new ASTValue(new LangNil());
}
