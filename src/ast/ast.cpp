#include "ast.h"
#include <iostream>
#include <stdexcept>

ScopedSymbolTable* AST::scope;
/////////// AST
ASTValue AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
}

/////////// StatementListAST
ASTValue StatementListAST::solve() {
  if (this->scope == nullptr)
    this->scope = new ScopedSymbolTable();
  for (auto &statement : this->statements)
    statement->solve();
  return 0;
}

/////////// FunctionAST
ASTValue FunctionAST::solve() {
  FuncSymbol *func =
      new FuncSymbol(this->identifier.getValue(), this->statements);
  this->scope->set(func);
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
  switch (this->op.getValue()[0]) {
  case '+':
    if (std::holds_alternative<std::string>(leftValue))
      return std::get<std::string>(leftValue) +
             std::get<std::string>(right->solve());
    return std::get<int>(leftValue) + std::get<int>(this->right->solve());
  case '-':
    return std::get<int>(leftValue) - std::get<int>(this->right->solve());
  case '*':
    return std::get<int>(leftValue) * std::get<int>(this->right->solve());
  case '/':
    return std::get<int>(leftValue) / std::get<int>(this->right->solve());
  case '%':
    return std::get<int>(leftValue) % std::get<int>(this->right->solve());
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
    this->scope = this->scope->newScope();
    std::get<AST *>(value)->solve();
    this->scope = this->scope->previousScope;
    return 0;
  }
  return value;
}
/////////// NumberAST
ASTValue NumberAST::solve() { return std::stoi(this->token.getValue()); }

/////////// StringAST
ASTValue StringAST::solve() { return this->token.getValue(); }
