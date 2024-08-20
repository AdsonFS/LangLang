#include "symbol.h"
#include <stdexcept>

void ScopedSymbolTable::set(Symbol *symbol) {
  if (this->symbols.find(symbol->name) != this->symbols.end())
    throw std::runtime_error(
        "Error: ScopedSymbolTable::set() name already declared: " +
        symbol->name);
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

void ScopedSymbolTable::update(std::string name, ASTValue value) {
  if (this->symbols.find(name) != this->symbols.end()) {
    if (this->symbols[name]->value.index() != value.index())
      throw std::runtime_error(
          "Error: ScopedSymbolTable::update() type mismatch: " + name);
    this->symbols[name]->value = value;

  } else
    throw std::runtime_error(
        "Error: ScopedSymbolTable::update() name not found: " + name);
}

ASTValue ScopedSymbolTable::getValue(std::string name) {
  Symbol *symbol = this->getSymbol(name);
  if (symbol != nullptr)
    return symbol->value;
  throw std::runtime_error(
      "Error: ScopedSymbolTable::getValue() name not found: " + name);
}
