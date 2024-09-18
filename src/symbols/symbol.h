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

class ClassSymbol : public Symbol {
public:
  ClassSymbol(std::string name, ASTValue *value)
      : Symbol(name, value) {}
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
    this->set(new BuiltInTypeSymbol("number", new ASTValue(new LangNumber(0))));
    this->set(new BuiltInTypeSymbol("string", new ASTValue (new LangString(""))));
    this->set(new BuiltInTypeSymbol("void", new ASTValue (new LangVoid())));

    this->set(new BuiltInTypeSymbol("func",
                                    new ASTValue(new LangFunction(nullptr, new LangVoid(), this))));
  }
  std::string getName();
  bool set(Symbol *symbol);
  bool check(std::string name, int jumps);
  void remove(std::string name);
  /*ASTValue *update(std::string name, ASTValue *value, int jumps);*/
  ASTValue *getValue(std::string name, int jumps);
  Symbol *getSymbol(std::string name, int jumps);
  static bool isSameType(LangObject *lhs, LangObject *rhs);
  static int jumpTo(std::string name, ScopedSymbolTable *scope);

  static ScopedSymbolTable *newScopeByContext(ScopedSymbolTable *context,
                                              std::string scopeName,
                                              std::string identifier);
  ScopedSymbolTable *newScope(std::string scopeName) {
    return new ScopedSymbolTable(scopeName, this);
  }
  ScopedSymbolTable *previousScope;

  std::unordered_map<std::string, Symbol *> getSymbols() { return symbols; }
  void setSymbols(std::unordered_map<std::string, Symbol *> symbols) {
    this->symbols = symbols;
  }

private:
  std::string scopeName;
  std::unordered_map<std::string, Symbol *> symbols;
};

#endif // !SYMBOL_H
