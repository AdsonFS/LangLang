#ifndef VISITOR_H
#define VISITOR_H

#include "../core/core.h"

class ASTVisitor {
public:
  virtual ASTValue visitStatementList(StatementListAST *expr) = 0;
  virtual ASTValue visitBLock(BlockAST *expr) = 0;
  virtual ASTValue visitReturn(ReturnAST *expr) = 0;

  virtual ASTValue visitWhileStatement(WhileStatementAST *expr) = 0;
  virtual ASTValue visitForStatement(ForStatementAST *expr) = 0;
  virtual ASTValue visitIfStatement(IfStatementAST *expr) = 0;
  virtual ASTValue visitFunctionDeclaration(FunctionDeclarationAST *expr) = 0;

  virtual ASTValue visitOutputStream(OutputStreamAST *expr) = 0;
  virtual ASTValue visitInputStream(InputStreamAST *expr) = 0;

  virtual ASTValue visitVariableDeclaration(VariableDeclarationAST *expr) = 0;
  virtual ASTValue visitAssignmentVariable(AssignmentVariableAST *expr) = 0;

  virtual ASTValue visitBinaryOperatorExpr(BinaryOperatorAST *expr) = 0;
  virtual ASTValue visitUnaryOperatorExpr(UnaryOperatorAST *expr) = 0;

  virtual ASTValue visitCall(CallAST *expr) = 0;
  virtual ASTValue visitIdentifier(IdentifierAST *expr) = 0;
  virtual ASTValue visitNumberExpr(NumberAST *expr) = 0;
  virtual ASTValue visitStringExpr(StringAST *expr) = 0;

  virtual ASTValue visitNil(NilAST *expr) = 0;
};

#endif // VISITOR_H
