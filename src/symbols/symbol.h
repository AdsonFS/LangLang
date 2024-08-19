#ifndef SYMBOL_H
#define SYMBOL_H

#include "../dependencies/dependencies.h"
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
  BuiltInTypeSymbol(std::string name) : Symbol(name) {}
};

class FuncSymbol : public Symbol {
public:
  FuncSymbol(std::string name, ASTValue value = nullptr) : Symbol(name, nullptr, value) {}
};

class VarSymbol : public Symbol {
public:
  VarSymbol(std::string name, Symbol *type, ASTValue value) : Symbol(name, type, value) {}
};

///////////////////////////////////////////////////////////////

class ScopedSymbolTable {
public:
  ScopedSymbolTable(ScopedSymbolTable *previousScope = nullptr) : previousScope(previousScope) {
    this->set(new BuiltInTypeSymbol("NUMBER"));
    this->set(new BuiltInTypeSymbol("STRING"));
  }
  void set(Symbol *symbol);
  void update(std::string name, ASTValue value);
  ASTValue getValue(std::string name);
  Symbol *getSymbol(std::string name);
  
  ScopedSymbolTable *newScope() {
    return new ScopedSymbolTable(this);
  }
  ScopedSymbolTable *previousScope;
private:
  std::unordered_map<std::string, Symbol *> symbols;
};

#endif // !SYMBOL_H
