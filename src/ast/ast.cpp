#include "ast.h"
#include <iostream>
#include <stdexcept>

/////////// AST
std::variant<int, std::string> AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
}

/////////// StatementListAST
std::variant<int, std::string> StatementListAST::solve() {
  for (auto &statement : this->statements) 
    statement->solve();
  return 0;
}

/////////// VariableDeclarationAST
std::variant<int, std::string> VariableDeclarationAST::solve() {
  /*AST::hashTable[this->identifier] = 0;*/
  return 0;
}

/////////// OutputStreamAST
std::variant<int, std::string> OutputStreamAST::solve() {
  int cnt = this->outputs.size() - 1;
  std::cout << cnt << std::endl;
  for (auto &output : this->outputs) {
    std::variant<int, std::string> outputValue = output->solve();
    if (std::holds_alternative<std::string>(outputValue))
      std::cout << std::get<std::string>(outputValue);
    else
      std::cout << std::get<int>(outputValue);
    if (cnt--) std::cout << " ";
  }
  std::cout << "#" << std::endl;
  return (int)this->outputs.size();
}

/////////// BinaryOperatorAST
std::variant<int, std::string> BinaryOperatorAST::solve() {
  std::variant<int, std::string> leftValue = this->left->solve();
  switch (this->op.getValue()[0]) {
  case '+':
    if (std::holds_alternative<std::string>(leftValue))
      return std::get<std::string>(leftValue) +
             std::get<std::string>(right->solve());
    return std::get<int>(leftValue) +
           std::get<int>(this->right->solve());
  case '-':
    return std::get<int>(leftValue) -
           std::get<int>(this->right->solve());
  case '*':
    return std::get<int>(leftValue) *
           std::get<int>(this->right->solve());
  case '/':
    return std::get<int>(leftValue) /
           std::get<int>(this->right->solve());
  case '%':
    return std::get<int>(leftValue) %
           std::get<int>(this->right->solve());
  default:
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  }
}

/////////// UnaryOperatorAST
std::variant<int, std::string> UnaryOperatorAST::solve() {
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

/////////// NumberAST
std::variant<int, std::string> NumberAST::solve() {
  return std::stoi(this->token.getValue());
}

/////////// StringAST
std::variant<int, std::string> StringAST::solve() {
  return this->token.getValue();
}
