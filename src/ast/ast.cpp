#include "ast.h"
#include <stdexcept>

/////////// AST
std::variant<int, std::string> AST::solve() {
  throw std::runtime_error("Error: AST::solve() not implemented");
}

/////////// BinaryOperatorAST
std::variant<int, std::string> BinaryOperatorAST::solve() {
  switch (this->op.getValue()[0]) {
  case '+':
    return std::get<int>(left->solve()) + std::get<int>(right->solve());
  case '-':
    return std::get<int>(left->solve()) - std::get<int>(right->solve());
  case '*':
    return std::get<int>(left->solve()) * std::get<int>(right->solve());
  case '/':
    return std::get<int>(left->solve()) / std::get<int>(right->solve());
  case '%':
    return std::get<int>(left->solve()) % std::get<int>(right->solve());
  default:
    throw std::runtime_error(
        "Error: BinaryOperatorAST::solve() invalid operator");
  }
}

/////////// NumberAST
std::variant<int, std::string> NumberAST::solve() {
  return std::stoi(this->token.getValue());
}
