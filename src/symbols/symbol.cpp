#include "symbol.h"
#include "../error/error.h"
#include <iostream>

std::string ScopedSymbolTable::getName() { return this->scopeName; }

ScopedSymbolTable *
ScopedSymbolTable::newScopeByContext(std::string scopeName,
                                     std::string identifier) {
  ScopedSymbolTable *currentScope = this;
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
  return nullptr;
}

ASTValue *ScopedSymbolTable::update(std::string name, ASTValue *value) {
  // TODO
  ScopedSymbolTable *scope =
      this->newScopeByContext(this->scopeName, name)->previousScope;
  if (scope->symbols.find(name) != scope->symbols.end()) {
    if (!LangObject::isSameType(scope->symbols[name]->type->value, value))
      throw RuntimeError("type mismatch...: " + name);
    scope->symbols[name]->value = value;

  } else
    throw RuntimeError("name not found: " + name);
  return value;
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
