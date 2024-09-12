#ifndef LANG_OBJECT_H
#define LANG_OBJECT_H

#include "../error/error.h"
#include "istream"
#include <string>
#include <typeinfo>

class AST;
class LangBoolean;
class LangFunction;
class ScopedSymbolTable;

class LangObject {
private:
public:
  /*static bool isSameType(const LangObject *a, const LangObject *b) {*/
    /*return typeid(*a) == typeid(*b);*/
  /*}*/

  virtual bool isTrue() const = 0;
  virtual void cin(std::istream &is) {
    throw RuntimeError("Cannot read value: LangObject::cin()");
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
  LangFunction(AST *value, LangObject *returnType, ScopedSymbolTable *scope)
      : value(value), returnType(returnType), scope(scope) {}
  void setValue(AST *value) { this->value = value; }
  AST *getValue() { return value; }
  LangObject *getReturnType() { return returnType; }
  ScopedSymbolTable *getScope() { return scope; }

private:
  bool isTrue() const override { return true; }
  void toString(std::ostream &os) const override;
  AST *value;
  ScopedSymbolTable *scope;
  LangObject *returnType;
};

class LangString : public LangObject {
public:
  LangString(std::string value) : value(value) {}

private:
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
  LangObject *operator-() override;
  LangObject *operator+() override;

  LangBoolean *operator<(const LangObject &rhs) const override {
    return new LangBoolean(value < ((LangNumber &)rhs).value);
  }
  LangObject *operator+(const LangObject &rhs) const override {
    return new LangNumber(value + ((LangNumber &)rhs).value);
  }

  bool isTrue() const override { return value; }
  void toString(std::ostream &os) const override;
  void cin(std::istream &is) override;
  int value;
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

typedef LangObject ASTValue;

#endif // LANG_OBJECT_H
