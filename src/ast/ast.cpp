#include "ast.h"
#include <iostream>
#include <stdexcept>

std::unordered_map<std::string, ASTValue> AST::hashTable;
/////////// AST
ASTValue AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
}

/////////// StatementListAST
ASTValue StatementListAST::solve() {
  for (auto &statement : this->statements)
    statement->solve();
  return 0;
}

/////////// VariableDeclarationAST
ASTValue VariableDeclarationAST::solve() {
  if (this->hashTable.find(this->identifier.getValue()) != this->hashTable.end())
    throw std::runtime_error("Error: VariableDeclarationAST::solve() identifier already declared: " + this->identifier.getValue());
  this->hashTable[this->identifier.getValue()] = this->value->solve();
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
  // verify if the identifier is in the hashTable
  if (this->hashTable.find(this->token.getValue()) == this->hashTable.end())
    throw std::runtime_error("Error: IdentifierAST::solve() identifier not found: " + this->token.getValue());
  return this->hashTable[this->token.getValue()];
}
/////////// NumberAST
ASTValue NumberAST::solve() {
  return std::stoi(this->token.getValue());
}

/////////// StringAST
ASTValue StringAST::solve() {
  return this->token.getValue();
}
