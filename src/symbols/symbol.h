#ifndef SYMBOL_H
#define SYMBOL_H

#include "../core/core.h"
#include <string>
#include <unordered_map>

class ScopedSymbolTable;

class Symbol {
public:
  Symbol(std::string name, ScopedSymbolTable *context, ASTValue *value)
      : name(name), context(context), value(value) {}
  std::string name;
  ScopedSymbolTable *context;
  ASTValue *value;
};

class BuiltInTypeSymbol : public Symbol {
public:
  BuiltInTypeSymbol(std::string name, ScopedSymbolTable *context,
                    ASTValue *value)
      : Symbol(name, context, value) {}
};

class FuncSymbol : public Symbol {
public:
  FuncSymbol(std::string name, ScopedSymbolTable *context, ASTValue *value)
      : Symbol(name, context, value) {}
};

class VarSymbol : public Symbol {
public:
  VarSymbol(std::string name, ScopedSymbolTable *context, ASTValue *value)
      : Symbol(name, context, value) {}
};

///////////////////////////////////////////////////////////////

class ScopedSymbolTable {
public:
  ScopedSymbolTable(std::string scopeName,
                    ScopedSymbolTable *previousScope = nullptr)
      : scopeName(scopeName), previousScope(previousScope) {
    this->set(new BuiltInTypeSymbol("number", this, new LangNumber(0)));
    this->set(new BuiltInTypeSymbol("string", this, new LangString("")));
    this->set(new BuiltInTypeSymbol("void", this, new LangVoid()));

    this->set(new BuiltInTypeSymbol("func", this,
                                    new LangFunction(nullptr, new LangVoid())));
  }
  std::string getName();
  void set(Symbol *symbol);
  ASTValue *update(std::string name, ASTValue *value);
  ASTValue *getValue(std::string name);
  Symbol *getSymbol(std::string name);
  bool isSameType(ASTValue *lhs, ASTValue *rhs);

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
