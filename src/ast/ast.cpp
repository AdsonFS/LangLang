#include "ast.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>

ScopedSymbolTable *AST::scope;
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
ASTValue IfStatementAST::solve() {
  ASTValue value = this->condition->solve();
  bool condition = std::holds_alternative<int>(value)
                       ? std::get<int>(value)
                       : std::get<std::string>(value) != "";

  if (condition) {
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

  case '<':
    if (this->isParenthesized)
      return leftValue < rightValue;
    return leftValue < rightValue ? rightValue : 0;
  case '>':
    if (this->isParenthesized)
      return leftValue > rightValue;
    return leftValue > rightValue ? rightValue : 0;
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
