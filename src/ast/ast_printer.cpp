#include "ast_printer.h"

int ASTPrinter::indent = 0;

void ASTPrinter::printIndent(int indent) {
  for (int i = 0; i < indent; i++)
    std::cout << "\t";
  std::cout << "-> ";
}

ASTValue ASTPrinter::visitStatementList(StatementListAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<StatementListAST>\n";
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitWhileStatement(WhileStatementAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<WhileStatementAST>\n";
  this->printIndent(this->indent++);
  std::cout << "<Condition>\n";
  expr->condition->accept(*this);
  this->indent--;
  expr->ifStatements->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitIfStatement(IfStatementAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<IfStatementAST>\n";
  this->printIndent(this->indent++);
  std::cout << "<Condition>\n";
  expr->condition->accept(*this);
  this->indent--;
  expr->ifStatements->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitFunction(FunctionAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<FunctionAST:" << expr->identifier.getValue() << ">\n";
  expr->statements->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitOutputStream(OutputStreamAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<OutputStreamAST>\n";
  for (auto &output : expr->outputs)
    output->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitInputStream(InputStreamAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<InputStreamAST>\n";
  for (auto &identifier : expr->identifiers) {
    this->printIndent(this->indent);
    std::cout << "<IdentifierAST:" << identifier.getValue() << ">\n";
  }
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitVariableDeclaration(VariableDeclarationAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<VariableDeclarationAST:" << expr->identifier.getValue()
            << ">\n";
  expr->value->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitAssignmentVariable(AssignmentVariableAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<AssignmentVariableAST:" << expr->identifier.getValue()
            << ">\n";
  expr->value->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<BinaryOperatorAST:" << expr->op.getValue() << ">\n";
  expr->left->accept(*this);
  expr->right->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<UnaryOperatorAST:" << expr->op.getValue() << ">\n";
  expr->child->accept(*this);
  this->indent--;
  return 0;
}

ASTValue ASTPrinter::visitIdentifier(IdentifierAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<IdentifierAST:" << expr->token.getValue() << ">\n";
  return 0;
}

ASTValue ASTPrinter::visitNumberExpr(NumberAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<NumberAST:" << expr->token.getValue() << ">\n";
  return 0;
}

ASTValue ASTPrinter::visitStringExpr(StringAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<StringAST:" << expr->token.getValue() << ">\n";
  return 0;
}
