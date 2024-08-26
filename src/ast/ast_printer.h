#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "ast.h"
#include "visitor.h"
#include <iostream>

class ASTPrinter : public ASTVisitor {
public:
  static int indent;
private:
  void printIndent(int indent);

  ASTValue visitStatementList(StatementListAST *expr) override; 

  ASTValue visitWhileStatement(WhileStatementAST *expr) override; 

  ASTValue visitIfStatement(IfStatementAST *expr) override; 

  ASTValue visitFunction(FunctionAST *expr) override; 

  ASTValue visitOutputStream(OutputStreamAST *expr) override; 

  ASTValue visitInputStream(InputStreamAST *expr) override; 
  
  ASTValue visitVariableDeclaration(VariableDeclarationAST *expr) override; 

  ASTValue visitAssignmentVariable(AssignmentVariableAST *expr) override; 

  ASTValue visitBinaryOperatorExpr(BinaryOperatorAST *expr) override; 

  ASTValue visitUnaryOperatorExpr(UnaryOperatorAST *expr) override; 

  ASTValue visitIdentifier(IdentifierAST *expr) override; 

  ASTValue visitNumberExpr(NumberAST *expr) override; 

  ASTValue visitStringExpr(StringAST *expr) override; 
};
#endif // !AST_PRINTER_H
#define AST_PRINTER_H
