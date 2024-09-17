#ifndef INTERPRETER_VISITOR_H
#define INTERPRETER_VISITOR_H

#include "ast.h"
#include "visitor.h"
#include <iostream>

class InterpreterVisitor: public ASTVisitor {
public:
  static void setJumpTable(std::unordered_map<AST*, int> jumpTable) {
    InterpreterVisitor::jumpTable = jumpTable;
  }
protected:
private:
  static ScopedSymbolTable *scope;
  static std::unordered_map<AST*, int> jumpTable;

  ASTValue *visitCall(LangObject *callee, std::string name);

  ASTValue* visitStatementList(StatementListAST *expr) override; 

  ASTValue* visitBLock(BlockAST *expr) override;

  ASTValue* visitReturn(ReturnAST *expr) override;

  ASTValue* visitWhileStatement(WhileStatementAST *expr) override; 

  ASTValue* visitForStatement(ForStatementAST *expr) override; 

  ASTValue* visitIfStatement(IfStatementAST *expr) override; 

  ASTValue* visitClassDeclaration(ClassDeclarationAST *expr) override;

  ASTValue* visitFunctionDeclaration(FunctionDeclarationAST *expr) override; 

  ASTValue* visitOutputStream(OutputStreamAST *expr) override; 

  ASTValue* visitInputStream(InputStreamAST *expr) override; 
  
  ASTValue* visitVariableDeclaration(VariableDeclarationAST *expr) override; 

  ASTValue* visitAssignmentVariable(AssignmentVariableAST *expr) override; 

  ASTValue* visitBinaryOperatorExpr(BinaryOperatorAST *expr) override; 

  ASTValue* visitUnaryOperatorExpr(UnaryOperatorAST *expr) override; 

  ASTValue* visitCall(CallAST *expr) override;

  ASTValue* visitPropertyChain(PropertyChainAST *expr) override;

  ASTValue* visitType(TypeAST* expr) override;

  ASTValue* visitIdentifier(IdentifierAST *expr) override; 

  ASTValue* visitNumberExpr(NumberAST *expr) override; 

  ASTValue* visitStringExpr(StringAST *expr) override; 

  ASTValue* visitVoid(VoidAST *expr) override;

  ASTValue* visitNil(NilAST *expr) override;
};

#endif // INTERPRETER_VISITOR_H
