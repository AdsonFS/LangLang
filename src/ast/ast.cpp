#include "ast.h"
#include <stdexcept>

/////////// AST
std::variant<int, std::string> AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
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
