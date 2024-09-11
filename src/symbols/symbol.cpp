#include "symbol.h"
#include "../error/error.h"

std::string ScopedSymbolTable::getName() { return this->scopeName; }

bool ScopedSymbolTable::isSameType(ASTValue *lhs, ASTValue *rhs) {
  if (typeid(*lhs) != typeid(*rhs))
    return false;
  if (typeid(*lhs) == typeid(LangFunction)) {
    LangFunction *lhsFunc = dynamic_cast<LangFunction *>(lhs);
    LangFunction *rhsFunc = dynamic_cast<LangFunction *>(rhs);

    if (lhsFunc->getReturnType() == nullptr || rhsFunc->getReturnType() == nullptr)
      return true;
    return isSameType(lhsFunc->getReturnType(), rhsFunc->getReturnType());
  }
  return true;
}

ScopedSymbolTable *
ScopedSymbolTable::newScopeByContext(ScopedSymbolTable* context, 
                                    std::string scopeName,
                                     std::string identifier) {
  ScopedSymbolTable *currentScope = context;
  while (currentScope != nullptr) {
    if (currentScope->symbols.find(identifier) != currentScope->symbols.end())
      return new ScopedSymbolTable(scopeName, currentScope);
    currentScope = currentScope->previousScope;
  }
  throw RuntimeError("name not found: " + identifier);
}

void ScopedSymbolTable::set(Symbol *symbol) {
  if (this->symbols.find(symbol->name) != this->symbols.end())
    throw RuntimeError("name already declared: " + symbol->name);
  this->symbols[symbol->name] = symbol;
}

Symbol *ScopedSymbolTable::getSymbol(std::string name) {
  ScopedSymbolTable *currentScope = this;
  while (currentScope != nullptr) {
    if (currentScope->symbols.find(name) != currentScope->symbols.end())
      return currentScope->symbols[name];
    currentScope = currentScope->previousScope;
  }
  throw RuntimeError("name not found: " + name);
  /*return nullptr;*/
}

ASTValue *ScopedSymbolTable::update(std::string name, ASTValue *value) {
  // TODO
  ScopedSymbolTable *scope = this;
  while (scope != nullptr) {
    if (scope->symbols.find(name) != scope->symbols.end())
      break;
    scope = scope->previousScope;
  }
  if (scope == nullptr)
    throw RuntimeError("name not found: " + name);

  if (dynamic_cast<LangNil *>(scope->symbols[name]->value) != nullptr) {
    LangNil *nil = dynamic_cast<LangNil *>(scope->symbols[name]->value);
    
    if (nil->getType() == nullptr || !this->isSameType(nil->getType(), value))
      throw RuntimeError("type mismatch...: " + name);
  } else if (!this->isSameType(scope->symbols[name]->value, value))
    throw RuntimeError("type mismatch...: " + name);
  return scope->symbols[name]->value = value;
}

ASTValue *ScopedSymbolTable::getValue(std::string name) {
  // Todo
  Symbol *symbol = this->getSymbol(name);
  if (symbol == nullptr)
    throw RuntimeError("name not found: " + name);
  if (dynamic_cast<LangNil *>(symbol->value) != nullptr)
    throw RuntimeError("variable not initialized: " + name);
  return symbol->value;
}
