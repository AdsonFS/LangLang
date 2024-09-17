#ifndef CORE_H
#define CORE_H

#include <string>
#include <variant>
#include "lang_object.h"

class ASTVisitor;

class AST;
class StatementListAST;
class WhileStatementAST;
class ForStatementAST;
class BlockAST;
class ReturnAST;
class IfStatementAST;
class ClassDeclarationAST;
class FunctionDeclarationAST;
class OutputStreamAST;
class InputStreamAST;
class VariableDeclarationAST;
class AssignmentVariableAST;
class BinaryOperatorAST;
class UnaryOperatorAST;
class CallAST;
class PropertyChainAST;
class IdentifierAST;
class NumberAST;
class StringAST;
class VoidAST;
class NilAST;

/*typedef LangObject ASTValue;*/

#endif // CORE_H
