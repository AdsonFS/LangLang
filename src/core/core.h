#ifndef CORE_H
#define CORE_H

#include <string>
#include <variant>

class ASTVisitor;

class AST;
class StatementListAST;
class WhileStatementAST;
class ForStatementAST;
class BlockAST;
class ReturnAST;
class IfStatementAST;
class FunctionDeclarationAST;
class OutputStreamAST;
class InputStreamAST;
class VariableDeclarationAST;
class AssignmentVariableAST;
class BinaryOperatorAST;
class UnaryOperatorAST;
class CallAST;
class IdentifierAST;
class NumberAST;
class StringAST;
class VoidAST;
class NilAST;

class VoidValue {
public:
  bool operator!=(const VoidValue &rhs) const { return false; }
  bool operator==(const VoidValue &rhs) const { return false; }
  bool operator<(const VoidValue &rhs) const { return false; }
  bool operator>(const VoidValue &rhs) const { return false; }
};

typedef std::variant<int, std::string, VoidValue, AST *> ASTValue;
bool isSameASTValueType(const ASTValue &a, ASTValue &b);

#endif // CORE_H
