#ifndef SYMBOL_H
#define SYMBOL_H

#include "../core/core.h"
#include <string>
#include <unordered_map>

class ScopedSymbolTable;

class Symbol {
public:
  Symbol(std::string name, ASTValue *value)
      : name(name),  value(value) {}
  std::string name;
  ASTValue *value;
};

class BuiltInTypeSymbol : public Symbol {
public:
  BuiltInTypeSymbol(std::string name, 
                    ASTValue *value)
      : Symbol(name,  value) {}
};

class FuncSymbol : public Symbol {
public:
  FuncSymbol(std::string name, ASTValue *value)
      : Symbol(name, value) {}
};

class VarSymbol : public Symbol {
public:
  VarSymbol(std::string name, ASTValue *value)
      : Symbol(name, value) {}
};

///////////////////////////////////////////////////////////////

class ScopedSymbolTable {
public:
  ScopedSymbolTable(std::string scopeName,
                    ScopedSymbolTable *previousScope = nullptr)
      : scopeName(scopeName), previousScope(previousScope) {
    this->set(new BuiltInTypeSymbol("number", new LangNumber(0)));
    this->set(new BuiltInTypeSymbol("string", new LangString("")));
    this->set(new BuiltInTypeSymbol("void", new LangVoid()));

    this->set(new BuiltInTypeSymbol("func",
                                    new LangFunction(nullptr, new LangVoid(), this)));
  }
  std::string getName();
  void set(Symbol *symbol);
  ASTValue *update(std::string name, ASTValue *value, int jumps);
  ASTValue *getValue(std::string name, int jumps);
  Symbol *getSymbol(std::string name, int jumps);
  bool isSameType(ASTValue *lhs, ASTValue *rhs);
  static int jumpTo(std::string name, ScopedSymbolTable *scope);

  static ScopedSymbolTable *newScopeByContext(ScopedSymbolTable *context,
                                              std::string scopeName,
                                              std::string identifier);
  ScopedSymbolTable *newScope(std::string scopeName) {
    return new ScopedSymbolTable(scopeName, this);
  }
  ScopedSymbolTable *previousScope;

private:
  std::string scopeName;
  std::unordered_map<std::string, Symbol *> symbols;
};

#endif // !SYMBOL_H
