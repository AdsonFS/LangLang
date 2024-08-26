#include "interpreter_visitor.h"

ScopedSymbolTable *InterpreterVisitor::scope;

bool InterpreterVisitor::ASTValueIsTrue(ASTValue value) {
  return std::holds_alternative<int>(value)
             ? std::get<int>(value) != 0
             : std::get<std::string>(value) != "";
}

ASTValue InterpreterVisitor::visitStatementList(StatementListAST *expr) {
  if (this->scope == nullptr) this->scope = new ScopedSymbolTable("global");
  for (auto &statement : expr->statements)
    statement->accept(*this);
  return 0;
}

ASTValue InterpreterVisitor::visitWhileStatement(WhileStatementAST *expr) {
  while (ASTValueIsTrue(expr->condition->accept(*this))) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return 0;
}

ASTValue InterpreterVisitor::visitIfStatement(IfStatementAST *expr) {
  ASTValue value = expr->condition->accept(*this);

  if (ASTValueIsTrue(value)) {
    this->scope = this->scope->newScope("if");
    expr->ifStatements->accept(*this);
    this->scope = this->scope->previousScope;
  }
  return 0;
}

ASTValue InterpreterVisitor::visitFunction(FunctionAST *expr) {
  FuncSymbol *func = new FuncSymbol(expr->identifier.getValue(), expr->statements);
  this->scope->set(func);
  return 0;
}

ASTValue InterpreterVisitor::visitOutputStream(OutputStreamAST *expr) {
  int cnt = expr->outputs.size() - 1;
  for (auto &output : expr->outputs) {
    ASTValue outputValue = output->accept(*this);
    if (std::holds_alternative<std::string>(outputValue))
      std::cout << std::get<std::string>(outputValue);
    else
      std::cout << std::get<int>(outputValue);
    if (cnt--)
      std::cout << " ";
  }
  std::cout << std::endl;
  return (int)expr->outputs.size();
}

ASTValue InterpreterVisitor::visitInputStream(InputStreamAST *expr) {
  for (auto &identifier : expr->identifiers) {
    ASTValue value = this->scope->getValue(identifier.getValue());
    if (std::holds_alternative<int>(value)) {
      int input;
      std::cin >> input;
      this->scope->update(identifier.getValue(), input);
    } else if (std::holds_alternative<std::string>(value)) {
      std::string input;
      std::cin >> input;
      this->scope->update(identifier.getValue(), input);
    } else {
      throw std::runtime_error(
          "Error: InputStreamAST::solve() invalid identifier");
    }
  }
  return 0;
}

ASTValue InterpreterVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  Token type = expr->type;
  if (type.getValue() == "NUMBER")
    this->scope->set(new VarSymbol(expr->identifier.getValue(),
                                   this->scope->getSymbol("NUMBER"),
                                   expr->value->accept(*this)));
  else if (type.getValue() == "STRING")
    this->scope->set(new VarSymbol(expr->identifier.getValue(),
                                   this->scope->getSymbol("STRING"),
                                   expr->value->accept(*this)));
  else
    throw std::runtime_error(
        "Error: VariableDeclarationAST::solve() invalid type");
  return 0;
}

ASTValue InterpreterVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  this->scope->update(expr->identifier.getValue(), expr->value->accept(*this));
  return 0;
}

ASTValue InterpreterVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  ASTValue leftValue = expr->left->accept(*this);
  ASTValue rightValue = expr->right->accept(*this);
  switch (expr->op.getValue()[0]) {
  case '+':
    if (std::holds_alternative<std::string>(leftValue))
      return std::get<std::string>(leftValue) +
             std::get<std::string>(rightValue);
    return std::get<int>(leftValue) + std::get<int>(rightValue);
  case '-':
    return std::get<int>(leftValue) - std::get<int>(rightValue);
  case '*':
    return std::get<int>(leftValue) * std::get<int>(rightValue);
  case '/':
    return std::get<int>(leftValue) / std::get<int>(rightValue);
  case '%':
    return std::get<int>(leftValue) % std::get<int>(rightValue);

  // logical operators
  case '&':
    if (expr->op.getValue() == "&&")
      return ASTValueIsTrue(leftValue) && ASTValueIsTrue(rightValue);
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  case '|':
    if (expr->op.getValue() == "||")
      return ASTValueIsTrue(leftValue) || ASTValueIsTrue(rightValue);
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  case '<':
    return leftValue < rightValue;
  case '>':
    return leftValue > rightValue;
    ;
  default:
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  }
}

ASTValue InterpreterVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  switch (expr->op.getValue()[0]) {
  case '+':
    return std::get<int>(expr->child->accept(*this));
  case '-':
    return -std::get<int>(expr->child->accept(*this));
  default:
    throw std::runtime_error(
        "Error: UnaryOperatorAST::solve() invalid operator");
  }
}

ASTValue InterpreterVisitor::visitIdentifier(IdentifierAST *expr) {

  ASTValue value = this->scope->getValue(expr->token.getValue());
  if (std::holds_alternative<AST *>(value)) { // function call
    ScopedSymbolTable *currentScope = this->scope;
    this->scope = this->scope->newScopeByContext(this->scope->getName() +
                                                     expr->token.getValue(),
                                                 expr->token.getValue());
    std::get<AST *>(value)->accept(*this);
    this->scope = currentScope;
    return 0;
  }
  return value;
}

ASTValue InterpreterVisitor::visitNumberExpr(NumberAST * expr) 
{ return std::stoi(expr->token.getValue()); }

ASTValue InterpreterVisitor::visitStringExpr(StringAST * expr) { return expr->token.getValue(); }