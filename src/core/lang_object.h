#ifndef LANG_OBJECT_H
#define LANG_OBJECT_H

#include "../error/error.h"
#include "istream"
#include <string>
#include <typeinfo>
#include <vector>

class AST;
class VariableDeclarationAST;
class LangBoolean;
class LangFunction;
class ScopedSymbolTable;

class LangObject {
private:
public:
  virtual bool isTrue() const = 0;
  virtual void cin(std::istream &is) {
    throw RuntimeError("Cannot read value: LangObject::cin()");
  }
  virtual void setValue(LangObject *value) {
    throw RuntimeError("Cannot set value: LangObject::setValue()");
  }
  virtual void toString(std::ostream &os) const = 0;

  friend std::istream &operator>>(std::istream &is, LangObject &value);
  friend std::ostream &operator<<(std::ostream &os, const LangObject &value);

  // unary operators
  virtual LangObject *operator-() {
    throw RuntimeError("Invalid operation: LangObject::operator-()");
  }
  virtual LangObject *operator+() {
    throw RuntimeError("Invalid operation: LangObject::operator+()");
  }

  // binary operators
  virtual LangBoolean *operator==(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator==()");
  }
  virtual LangBoolean *operator!=(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator!=()");
  }
  virtual LangBoolean *operator<(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator<()");
  }
  virtual LangBoolean *operator>(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator>()");
  }

  virtual LangObject *operator+(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator+()");
  }
  virtual LangObject *operator-(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator-()");
  }
  virtual LangObject *operator*(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator*()");
  }
  virtual LangObject *operator/(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator/()");
  }
  virtual LangObject *operator%(const LangObject &rhs) const {
    throw RuntimeError("Invalid operation: LangObject::operator%()");
  }
};

class LangFunction : public LangObject {
public:
  LangFunction(AST *value, std::vector<LangObject *> arguments,
               std::vector<VariableDeclarationAST *> parameters, LangObject *returnType,
               ScopedSymbolTable *scope)
      : value(value), arguments(arguments), parameters(parameters),
        returnType(returnType), scope(scope) {}
  void setValue(AST *value) { this->value = value; }
  AST *getValue() { return value; }
  LangObject *getReturnType() { return returnType; }
  ScopedSymbolTable *getScope() { return scope; }
  std::vector<LangObject *> getArguments() { return arguments; }
  std::vector<VariableDeclarationAST *> getParameters() { return parameters; }

private:
  void setValue(LangObject *value) override {
    LangFunction *func = (LangFunction *)value;
    this->value = func->value;
    this->scope = func->scope;
    this->returnType = func->returnType;
  }
  bool isTrue() const override { return true; }
  void toString(std::ostream &os) const override;
  AST *value;
  std::vector<VariableDeclarationAST *> parameters;
  std::vector<LangObject *> arguments;
  ScopedSymbolTable *scope;
  LangObject *returnType;
};

class LangString : public LangObject {
public:
  LangString(std::string value) : value(value) {}

private:
  void setValue(LangObject *value) override {
    this->value = ((LangString *)value)->value;
  }
  LangBoolean *operator==(const LangObject &rhs) const override;
  bool isTrue() const override { return !value.empty(); }
  void toString(std::ostream &os) const override;
  void cin(std::istream &is) override;
  std::string value;
};

class LangBoolean : public LangObject {
public:
  LangBoolean(bool value) : value(value) {}

private:
  bool isTrue() const override { return value; }
  void toString(std::ostream &os) const override;
  void cin(std::istream &is) override;
  bool value;
};

class LangNumber : public LangObject {
public:
  LangNumber(int value) : value(value) {}

private:
  void setValue(LangObject *value) override {
    this->value = ((LangNumber *)value)->value;
  }

  LangObject *operator-() override;
  LangObject *operator+() override;

  LangBoolean *operator==(const LangObject &rhs) const override {
    return new LangBoolean(value == ((LangNumber &)rhs).value);
  }

  LangBoolean *operator<(const LangObject &rhs) const override {
    return new LangBoolean(value < ((LangNumber &)rhs).value);
  }
  LangBoolean *operator>(const LangObject &rhs) const override {
    return new LangBoolean(value > ((LangNumber &)rhs).value);
  }
  LangObject *operator+(const LangObject &rhs) const override {
    return new LangNumber(value + ((LangNumber &)rhs).value);
  }
  LangObject *operator-(const LangObject &rhs) const override {
    return new LangNumber(value - ((LangNumber &)rhs).value);
  }

  bool isTrue() const override { return value; }
  void toString(std::ostream &os) const override;
  void cin(std::istream &is) override;
  int value;
};

class LangClass : public LangObject {
public:
  LangClass(std::string name, ScopedSymbolTable *scope)
      : name(name), scope(scope) {}
  ScopedSymbolTable *getScope() { return scope; }

private:
  bool isTrue() const override { return true; }
  void toString(std::ostream &os) const override;
  std::string name;
  ScopedSymbolTable *scope;
};

class LangVoid : public LangObject {
public:
  LangVoid() {}

  bool operator!=(const LangVoid &rhs) const { return true; }
  bool operator==(const LangVoid &rhs) const { return false; }
  bool operator<(const LangVoid &rhs) const { return false; }
  bool operator>(const LangVoid &rhs) const { return false; }

private:
  bool isTrue() const override { return false; }
  void toString(std::ostream &os) const override;
};

class LangNil : public LangObject {
public:
  LangNil(LangObject *type = nullptr) : type(type) {}
  LangObject *getType() { return type; }

  bool operator!=(const LangNil &rhs) const { return true; }
  bool operator==(const LangNil &rhs) const { return false; }
  bool operator<(const LangNil &rhs) const { return false; }
  bool operator>(const LangNil &rhs) const { return false; }

private:
  LangObject *type;
  bool isTrue() const override { return false; }
  void toString(std::ostream &os) const override;
};

/*typedef LangObject ASTValue;*/

#endif // LANG_OBJECT_H
