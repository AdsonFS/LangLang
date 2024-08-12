#include "expression_validator.h"

ExpressionValidator::ExpressionValidator(LexiScanner &scanner, Token &token)
    : scanner(scanner), token(token) {}

int ExpressionValidator::procedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/' || op == '%')
    return 2;
  return 0;
}

void ExpressionValidator::applyOpration(std::stack<int> &values, char op) {
  int right = values.top();
  values.pop();
  if (values.empty())
    throw std::runtime_error("Invalid expression");
  int left = values.top();
  values.pop();
  switch (op) {
  case '+':
    values.push(left + right);
    break;
  case '-':
    values.push(left - right);
    break;
  case '*':
    values.push(left * right);
    break;
  case '/':
    if (right == 0)
      throw std::runtime_error("Division by zero");
    values.push(left / right);
    break;
  case '%':
    values.push(left % right);
    break;
  }
}

int ExpressionValidator::validate() {
  std::stack<int> values;
  std::stack<char> operators;

  values.push(stoi(this->token.getValue()));

  while ((this->token = this->scanner.nextToken()).getType() == TK_OPERATOR) {

    while (!operators.empty() && procedence(operators.top()) >=
                                     procedence(this->token.getValue()[0])) {
      applyOpration(values, operators.top());
      operators.pop();
    }
    operators.push(this->token.getValue()[0]);

    this->token = this->scanner.nextToken();
    if (this->token.getType() != TK_NUMBER)
      throw std::runtime_error("Expected number but got: " +
                               this->token.getValue());
    values.push(stoi(this->token.getValue()));
  }

  while (!operators.empty()) {
    applyOpration(values, operators.top());
    operators.pop();
  }

  return values.top();
}
