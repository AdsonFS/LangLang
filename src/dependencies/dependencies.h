#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <string>
#include <variant>

class AST;
typedef std::variant<int, std::string, AST *> ASTValue;

#endif // DEPENDENCIES_H

