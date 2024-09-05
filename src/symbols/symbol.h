#ifndef SYMBOL_H
#define SYMBOL_H

#include "../core/core.h"
#include <string>
#include <unordered_map>

class Symbol {
public:
  Symbol(std::string name, Symbol *type = nullptr, ASTValue value = 0) : name(name), type(type), value(value) {}
  std::string name;
  Symbol *type;
  ASTValue value;
};

class BuiltInTypeSymbol : public Symbol {
public:
  BuiltInTypeSymbol(std::string name, ASTValue value) : Symbol(name, nullptr, value) {}
};

class FuncSymbol : public Symbol {
public:
  FuncSymbol(std::string name, Symbol *type, ASTValue value = nullptr) : Symbol(name, type, value) {}
};

class VarSymbol : public Symbol {
public:
  VarSymbol(std::string name, Symbol *type, ASTValue value) : Symbol(name, type, value) {}
};

///////////////////////////////////////////////////////////////

class ScopedSymbolTable {
public:
  ScopedSymbolTable(std::string scopeName, ScopedSymbolTable *previousScope = nullptr) : scopeName(scopeName), previousScope(previousScope) {
    this->set(new BuiltInTypeSymbol("number", 0));
    this->set(new BuiltInTypeSymbol("string", ""));
    this->set(new BuiltInTypeSymbol("func", VoidValue()));
  }
  std::string getName();
  void set(Symbol *symbol);
  void update(std::string name, ASTValue value);
  ASTValue getValue(std::string name);
  Symbol *getSymbol(std::string name);
  
  ScopedSymbolTable *newScopeByContext(std::string scopeName, std::string identifier);
  ScopedSymbolTable *newScope(std::string scopeName) {
    return new ScopedSymbolTable(scopeName, this);
  }
  ScopedSymbolTable *previousScope;
private:
  std::string scopeName;
  std::unordered_map<std::string, Symbol *> symbols;
};

#endif // !SYMBOL_H
