#include "core.h"

bool isSameASTValueType(const ASTValue &a, ASTValue &b) {
  return a.index() == b.index();
}
