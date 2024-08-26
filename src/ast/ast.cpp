#include "ast.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>

ScopedSymbolTable *AST::scope;

/////////// ASTVisitor
ASTValue AST::accept(ASTVisitor &visitor) { throw std::runtime_error("Error: AST::accept() not implemented"); }
ASTValue StatementListAST::accept(ASTVisitor &visitor) {
  return visitor.visitStatementList(this);
}
ASTValue WhileStatementAST::accept(ASTVisitor &visitor) {
  return visitor.visitWhileStatement(this);
}
ASTValue IfStatementAST::accept(ASTVisitor &visitor) {
  return visitor.visitIfStatement(this);
}
ASTValue FunctionAST::accept(ASTVisitor &visitor) {
  return visitor.visitFunction(this);
}
ASTValue OutputStreamAST::accept(ASTVisitor &visitor) {
  return visitor.visitOutputStream(this);
}
ASTValue InputStreamAST::accept(ASTVisitor &visitor) {
  return visitor.visitInputStream(this);
}
ASTValue VariableDeclarationAST::accept(ASTVisitor &visitor) {
  return visitor.visitVariableDeclaration(this);
}
ASTValue AssignmentVariableAST::accept(ASTVisitor &visitor) {
  return visitor.visitAssignmentVariable(this);
}
ASTValue BinaryOperatorAST::accept(ASTVisitor &visitor) {
  return visitor.visitBinaryOperatorExpr(this);
}
ASTValue UnaryOperatorAST::accept(ASTVisitor &visitor) {
  return visitor.visitUnaryOperatorExpr(this);
}
ASTValue IdentifierAST::accept(ASTVisitor &visitor) {
  return visitor.visitIdentifier(this);
}
ASTValue NumberAST::accept(ASTVisitor &visitor) {
  return visitor.visitNumberExpr(this);
}
ASTValue StringAST::accept(ASTVisitor &visitor) {
  return visitor.visitStringExpr(this);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/////////// AST
ASTValue AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
}

/////////// StatementListAST
ASTValue StatementListAST::solve() {
  if (this->scope == nullptr)
    this->scope = new ScopedSymbolTable("global");
  for (auto &statement : this->statements)
    statement->solve();
  return 0;
}

/////////// IfStatementAST
bool ASTValueIsTrue(ASTValue value) {
  return std::holds_alternative<int>(value)
             ? std::get<int>(value) != 0
             : std::get<std::string>(value) != "";
}

ASTValue IfStatementAST::solve() {
  ASTValue value = this->condition->solve();

  if (ASTValueIsTrue(value)) {
    this->scope = this->scope->newScope("if");
    this->ifStatements->solve();
    this->scope = this->scope->previousScope;
  }
  return 0;
}

ASTValue WhileStatementAST::solve() {

  while (ASTValueIsTrue(this->condition->solve())) {
    this->scope = this->scope->newScope("if");
    this->ifStatements->solve();
    this->scope = this->scope->previousScope;
  }
  return 0;
}

/////////// ConditionalAST
/*ASTValue ConditionalAST::solve() {*/
/*  ASTValue leftValue = this->left->solve();*/
/*  ASTValue rightValue = this->right->solve();*/
/*  if (leftValue.index() != rightValue.index())*/
/*    throw std::runtime_error(*/
/*        "Error: ConditionalAST::solve() different types");*/
/*  switch (this->op.getValue()[0]) {*/
/*    case '<':*/
/*      return leftValue < rightValue;*/
/*    case '>':*/
/*      return leftValue > rightValue;*/
/*    default:*/
/*      throw std::runtime_error(*/
/*          "Error: ConditionalAST::solve() invalid operator");*/
/*  }*/
/*}*/

/////////// FunctionAST
ASTValue FunctionAST::solve() {
  FuncSymbol *func =
      new FuncSymbol(this->identifier.getValue(), this->statements);
  this->scope->set(func);
  return 0;
}

/////////// AssignmentVariableAST
ASTValue AssignmentVariableAST::solve() {
  this->scope->update(this->identifier.getValue(), this->value->solve());
  return 0;
}

/////////// VariableDeclarationAST
ASTValue VariableDeclarationAST::solve() {
  Token type = this->type;
  if (type.getValue() == "NUMBER")
    this->scope->set(new VarSymbol(this->identifier.getValue(),
                                   this->scope->getSymbol("NUMBER"),
                                   this->value->solve()));
  else if (type.getValue() == "STRING")
    this->scope->set(new VarSymbol(this->identifier.getValue(),
                                   this->scope->getSymbol("STRING"),
                                   this->value->solve()));
  else
    throw std::runtime_error(
        "Error: VariableDeclarationAST::solve() invalid type");
  return 0;
}

/////////// InputStreamAST
ASTValue InputStreamAST::solve() {
  for (auto &identifier : this->identifiers) {
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

/////////// OutputStreamAST
ASTValue OutputStreamAST::solve() {
  int cnt = this->outputs.size() - 1;
  for (auto &output : this->outputs) {
    ASTValue outputValue = output->solve();
    if (std::holds_alternative<std::string>(outputValue))
      std::cout << std::get<std::string>(outputValue);
    else
      std::cout << std::get<int>(outputValue);
    if (cnt--)
      std::cout << " ";
  }
  std::cout << std::endl;
  return (int)this->outputs.size();
}

/////////// BinaryOperatorAST
ASTValue BinaryOperatorAST::solve() {
  ASTValue leftValue = this->left->solve();
  ASTValue rightValue = this->right->solve();
  switch (this->op.getValue()[0]) {
  case '+':
    if (std::holds_alternative<std::string>(leftValue))
      return std::get<std::string>(leftValue) +
             std::get<std::string>(right->solve());
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
    if (this->op.getValue() == "&&")
      return ASTValueIsTrue(leftValue) && ASTValueIsTrue(rightValue);
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  case '|':
    if (this->op.getValue() == "||")
      return ASTValueIsTrue(leftValue) || ASTValueIsTrue(rightValue);
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  case '<':

    /*if (this->isParenthesized)*/
    return leftValue < rightValue;
    /*return leftValue < rightValue ? rightValue : 0;*/
  case '>':
    /*if (this->isParenthesized)*/
    return leftValue > rightValue;
    /*return leftValue > rightValue ? rightValue : 0;*/
    ;
  default:
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  }
}

/////////// UnaryOperatorAST
ASTValue UnaryOperatorAST::solve() {
  switch (this->op.getValue()[0]) {
  case '+':
    return std::get<int>(this->child->solve());
  case '-':
    return -std::get<int>(this->child->solve());
  default:
    throw std::runtime_error(
        "Error: UnaryOperatorAST::solve() invalid operator");
  }
}

/////////// IdentifierAST
ASTValue IdentifierAST::solve() {
  ASTValue value = this->scope->getValue(this->token.getValue());
  if (std::holds_alternative<AST *>(value)) { // function call
    ScopedSymbolTable *currentScope = this->scope;
    this->scope = this->scope->newScopeByContext(this->scope->getName() +
                                                     this->token.getValue(),
                                                 this->token.getValue());
    std::get<AST *>(value)->solve();
    this->scope = currentScope;
    return 0;
  }
  return value;
}
/////////// NumberAST
ASTValue NumberAST::solve() { return std::stoi(this->token.getValue()); }

/////////// StringAST
ASTValue StringAST::solve() { return this->token.getValue(); }
