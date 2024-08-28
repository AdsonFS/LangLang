#ifndef CORE_H
#define CORE_H

#include <string>
#include <variant>

class ASTVisitor;

class AST;
class StatementListAST;
class WhileStatementAST;
class IfStatementAST;
class FunctionAST;
class OutputStreamAST;
class InputStreamAST;
class VariableDeclarationAST;
class AssignmentVariableAST;
class BinaryOperatorAST;
class UnaryOperatorAST;
class IdentifierAST;
class NumberAST;
class StringAST;

typedef std::variant<int, std::string, AST *> ASTValue;

#endif // CORE_H
