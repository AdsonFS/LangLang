#include "ast.h"
#include "../error/error.h"
#include "visitor.h"

ASTValue* AST::accept(ASTVisitor &visitor) {
  throw RuntimeError("not implemented");
}
ASTValue* StatementListAST::accept(ASTVisitor &visitor) {
  return visitor.visitStatementList(this);
}
ASTValue* BlockAST::accept(ASTVisitor &visitor) { return visitor.visitBLock(this); }
ASTValue* ReturnAST::accept(ASTVisitor &visitor) { return visitor.visitReturn(this); }
ASTValue* WhileStatementAST::accept(ASTVisitor &visitor) {
  return visitor.visitWhileStatement(this);
}
ASTValue* ForStatementAST::accept(ASTVisitor &visitor) {
  return visitor.visitForStatement(this);
}
ASTValue* IfStatementAST::accept(ASTVisitor &visitor) {
  return visitor.visitIfStatement(this);
}
ASTValue* ClassDeclarationAST::accept(ASTVisitor &visitor) {
  return visitor.visitClassDeclaration(this);
}
ASTValue* FunctionDeclarationAST::accept(ASTVisitor &visitor) {
  return visitor.visitFunctionDeclaration(this);
}
ASTValue* OutputStreamAST::accept(ASTVisitor &visitor) {
  return visitor.visitOutputStream(this);
}
ASTValue* InputStreamAST::accept(ASTVisitor &visitor) {
  return visitor.visitInputStream(this);
}
ASTValue* VariableDeclarationAST::accept(ASTVisitor &visitor) {
  return visitor.visitVariableDeclaration(this);
}
ASTValue* AssignmentVariableAST::accept(ASTVisitor &visitor) {
  return visitor.visitAssignmentVariable(this);
}
ASTValue* BinaryOperatorAST::accept(ASTVisitor &visitor) {
  return visitor.visitBinaryOperatorExpr(this);
}
ASTValue* UnaryOperatorAST::accept(ASTVisitor &visitor) {
  return visitor.visitUnaryOperatorExpr(this);
}
ASTValue* CallAST::accept(ASTVisitor &visitor) { return visitor.visitCall(this); }
ASTValue* PropertyChainAST::accept(ASTVisitor &visitor) {
  return visitor.visitPropertyChain(this);
}
ASTValue* IdentifierAST::accept(ASTVisitor &visitor) {
  return visitor.visitIdentifier(this);
}
ASTValue* NumberAST::accept(ASTVisitor &visitor) {
  return visitor.visitNumberExpr(this);
}
ASTValue* StringAST::accept(ASTVisitor &visitor) {
  return visitor.visitStringExpr(this);
}
ASTValue* VoidAST::accept(ASTVisitor &visitor) { return visitor.visitVoid(this); }
ASTValue* NilAST::accept(ASTVisitor &visitor) { return visitor.visitNil(this); }
