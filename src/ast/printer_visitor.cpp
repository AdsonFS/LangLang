#include "printer_visitor.h"

int PrinterVisitor::indent = 0;

void PrinterVisitor::printIndent(int indent) {
  for (int i = 0; i < indent; i++)
    std::cout << "\t";
  std::cout << "-> ";
}

ASTValue* PrinterVisitor::visitStatementList(StatementListAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<StatementListAST>\n";
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitBLock(BlockAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<BlockAST>\n";
  for (auto &statement : expr->statements)
    statement->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitReturn(ReturnAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<ReturnAST>\n";
  /*expr->value->accept(*this);*/
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitWhileStatement(WhileStatementAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<WhileStatementAST>\n";
  this->printIndent(this->indent++);
  std::cout << "<Condition>\n";
  expr->condition->accept(*this);
  this->indent--;
  expr->ifStatements->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitForStatement(ForStatementAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<ForStatementAST>\n";
  this->printIndent(this->indent++);
  std::cout << "<Initializer>\n";
  expr->initializer->accept(*this);
  this->printIndent(this->indent);
  std::cout << "<Condition>\n";
  expr->condition->accept(*this);
  this->printIndent(this->indent);
  std::cout << "<Increment>\n";
  expr->increment->accept(*this);
  this->indent--;
  expr->statements->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitIfStatement(IfStatementAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<IfStatementAST>\n";
  this->printIndent(this->indent++);
  std::cout << "<Condition>\n";
  expr->condition->accept(*this);
  this->indent--;
  expr->ifStatements->accept(*this);
  this->indent--;
  if (expr->elseStatements) {
    this->printIndent(this->indent++);
    std::cout << "<ElseStatement>\n";
    expr->elseStatements->accept(*this);
    this->indent--;
  }
  return new LangNil();
}

ASTValue* PrinterVisitor::visitFunctionDeclaration(FunctionDeclarationAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<FunctionDeclarationAST:" << expr->identifier.getValue() << ">\n";
    /*<< expr->type << ">\n";*/
  expr->statements->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitOutputStream(OutputStreamAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<OutputStreamAST>\n";
  for (auto &output : expr->outputs)
    output->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitInputStream(InputStreamAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<InputStreamAST>\n";
  for (auto &identifier : expr->identifiers) {
    this->printIndent(this->indent);
    std::cout << "<IdentifierAST:" << identifier.token.getValue() << ">\n";
  }
  this->indent--;
  return new LangNil();
}

ASTValue*
PrinterVisitor::visitVariableDeclaration(VariableDeclarationAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<VariableDeclarationAST:" << expr->identifier.getValue()
            << ">\n";
  expr->value->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitAssignmentVariable(AssignmentVariableAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<AssignmentVariableAST:" << expr->identifier.getValue()
            << ">\n";
  expr->value->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitBinaryOperatorExpr(BinaryOperatorAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<BinaryOperatorAST:" << expr->op.getValue() << ">\n";
  expr->left->accept(*this);
  expr->right->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitUnaryOperatorExpr(UnaryOperatorAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<UnaryOperatorAST:" << expr->op.getValue() << ">\n";
  expr->child->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitCall(CallAST *expr) {
  this->printIndent(this->indent++);
  std::cout << "<CallAST:" << expr->identifier.getValue() << ">\n";
  for (auto &argument : expr->arguments)
    argument->accept(*this);
  this->indent--;
  return new LangNil();
}

ASTValue* PrinterVisitor::visitIdentifier(IdentifierAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<IdentifierAST:" << expr->token.getValue() << ">\n";
  return new LangNil();
}

ASTValue* PrinterVisitor::visitNumberExpr(NumberAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<NumberAST:" << expr->token.getValue() << ">\n";
  return new LangNil();
}

ASTValue* PrinterVisitor::visitStringExpr(StringAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<StringAST:" << expr->token.getValue() << ">\n";
  return new LangNil();
}

ASTValue* PrinterVisitor::visitVoid(VoidAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<VoidAST>\n";
  return new LangNil();
}

ASTValue* PrinterVisitor::visitNil(NilAST *expr) {
  this->printIndent(this->indent);
  std::cout << "<NilAST>\n";
  return new LangNil();
}
