#ifndef INTERPRETER_VISITOR_H
#define INTERPRETER_VISITOR_H

#include "ast.h"
#include "visitor.h"
#include <iostream>

class InterpreterVisitor: public ASTVisitor {
public:
protected:
private:
  static ScopedSymbolTable *scope;
  bool ASTValueIsTrue(ASTValue value);

  ASTValue visitStatementList(StatementListAST *expr) override; 

  ASTValue visitBLock(BlockAST *expr) override;

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

  ASTValue visitNil(NilAST *expr) override;
};

#endif // INTERPRETER_VISITOR_H
