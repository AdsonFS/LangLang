#include "semantic_visitor.h"
#include "../core/lang_object.h"
#include "../error/error.h"

ScopedSymbolTable *SemanticVisitor::scope;
std::unordered_map<AST *, int> SemanticVisitor::jumpTable;
LangObject *SemanticVisitor::currentReturnType = nullptr;
LangObject *SemanticVisitor::currentFunctionType = nullptr;

ASTValue *SemanticVisitor::visitStatementList(StatementListAST *expr) {
  if (this->scope == nullptr)
    this->scope = new ScopedSymbolTable("global");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  return new LangNil();
}

ASTValue *SemanticVisitor::visitBLock(BlockAST *expr) {
  this->scope = this->scope->newScope("block");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->scope = this->scope->previousScope;
  return new LangNil();
}

ASTValue *SemanticVisitor::visitReturn(ReturnAST *expr) {
  currentReturnType = expr->value->accept(*this);
  return new LangNil();
}

ASTValue *SemanticVisitor::visitWhileStatement(WhileStatementAST *expr) {
  expr->condition->accept(*this);

  this->scope = this->scope->newScope("if");
  expr->ifStatements->accept(*this);
  this->scope = this->scope->previousScope;

  return new LangNil();
}

ASTValue *SemanticVisitor::visitForStatement(ForStatementAST *expr) {
  this->scope = this->scope->newScope("for");
  expr->initializer->accept(*this);

  expr->condition->accept(*this);
  expr->statements->accept(*this);
  expr->increment->accept(*this);

  this->scope = this->scope->previousScope;
  return new LangNil();
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

  return new LangNil();
}

ASTValue *
SemanticVisitor::visitFunctionDeclaration(FunctionDeclarationAST *expr) {

  ASTValue *type = nullptr;
  std::stack<Token> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = this->scope->getSymbol(types.top().getValue(), 0)->value;
    if (type == nullptr)
      type = t;
    else if (typeid(*t) != typeid(LangFunction))
      throw SemanticError("type mismatch: " + expr->identifier.getValue());
    else
      type = new LangFunction(nullptr, type, this->scope);
    types.pop();
  }

  FuncSymbol *func =
      new FuncSymbol(expr->identifier.getValue(),
                     new LangFunction(expr->statements, type, this->scope));
  if(!this->scope->set(func))
    throw SemanticError("Function " + expr->identifier.getValue() + " already declared");

  ScopedSymbolTable *currentScope = this->scope;

  this->scope = currentScope->newScope("functionin");
  this->currentReturnType = new LangVoid();
  this->currentFunctionType = type;
  
  expr->statements->accept(*this);

  if (!ScopedSymbolTable::isSameType(this->currentReturnType, type))
    throw SemanticError("Invalid return type to function: " + expr->identifier.getValue());

  this->scope = currentScope;
  return new LangNil();
}

ASTValue *SemanticVisitor::visitOutputStream(OutputStreamAST *expr) {
  for (auto &output : expr->outputs) {
    ASTValue *outputValue = output->accept(*this);
  }
  return new LangNil();
}

ASTValue *SemanticVisitor::visitInputStream(InputStreamAST *expr) {
  for (auto &identifier : expr->identifiers) {
    identifier.accept(*this);
  }
  return new LangNil();
}

ASTValue *
SemanticVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  ASTValue *value = expr->value->accept(*this);

  ASTValue *type = nullptr;
  std::stack<Token> types = expr->types;
  while (!types.empty()) {
    ASTValue *t = this->scope->getSymbol(types.top().getValue(), 0)->value;
    if (type == nullptr)
      type = t;
    else if (typeid(*t) != typeid(LangFunction))
      throw SemanticError("type mismatch: " + expr->identifier.getValue());
    else
      type = new LangFunction(nullptr, type, this->scope);
    types.pop();
  }

  if (dynamic_cast<LangNil *>(value) != nullptr)
    value = new LangNil(type);

  if(!this->scope->set(new VarSymbol(expr->identifier.getValue(), value)))
    throw SemanticError("Variable " + expr->identifier.getValue() + " already declared");
  return new LangNil();
}

ASTValue *
SemanticVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  ASTValue *value = expr->value->accept(*this);
  if(dynamic_cast<LangNil *>(value) != nullptr)
    value = dynamic_cast<LangNil *>(value)->getType();

  std::string name = expr->identifier.getValue();

  jumpTable[expr] = ScopedSymbolTable::jumpTo(name, this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Variable " + name + " not declared");

  ASTValue *symbol = this->scope->getSymbol(name, jumpTable[expr])->value; 
  if (dynamic_cast<LangNil *>(symbol) != nullptr) {
    LangNil *nil = dynamic_cast<LangNil *>(symbol);
    if (nil->getType() == nullptr || !ScopedSymbolTable::isSameType(nil->getType(), value))
      throw SemanticError("Type mismatch: " + name);
  } else if (!ScopedSymbolTable::isSameType(symbol, value))
    throw SemanticError("Type mismatch: " + name);


  return new LangNil();
}

ASTValue *SemanticVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  ASTValue* leftValue = expr->left->accept(*this);
  ASTValue* rightValue = expr->right->accept(*this);
  
  if (!ScopedSymbolTable::isSameType(leftValue, rightValue))
    throw SemanticError("Invalid type for binary operator");

  return leftValue;
}

ASTValue *SemanticVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  expr->child->accept(*this);
  return new LangNil();
}

ASTValue *SemanticVisitor::visitCall(CallAST *expr) {
  jumpTable[expr] =
      ScopedSymbolTable::jumpTo(expr->identifier.getValue(), this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Function " + expr->identifier.getValue() + " not declared");
  ASTValue *symbol = this->scope->getSymbol(expr->identifier.getValue(), jumpTable[expr])->value;
  if(typeid(*symbol) == typeid(LangNil))
    symbol = dynamic_cast<LangNil *>(symbol)->getType();

  
  if (typeid(*symbol) != typeid(LangFunction))
    throw SemanticError("Variable " + expr->identifier.getValue() + " is not a function");
  LangFunction *function = dynamic_cast<LangFunction *>(symbol);
  return function->getReturnType();
}

ASTValue *SemanticVisitor::visitIdentifier(IdentifierAST *expr) {
  jumpTable[expr] =
      ScopedSymbolTable::jumpTo(expr->token.getValue(), this->scope);
  if (jumpTable[expr] == -1)
    throw SemanticError("Variable " + expr->token.getValue() + " not declared");
  return this->scope->getSymbol(expr->token.getValue(), jumpTable[expr])->value;
}

ASTValue *SemanticVisitor::visitNumberExpr(NumberAST *expr) {
  return new LangNumber(0);
}

ASTValue *SemanticVisitor::visitStringExpr(StringAST *expr) {
  return new LangString("...");
}

ASTValue *SemanticVisitor::visitVoid(VoidAST *expr) { return new LangVoid(); }
ASTValue *SemanticVisitor::visitNil(NilAST *expr) { return new LangNil(); }
