#include "lang_object.h"

std::ostream &operator<<(std::ostream &os, const LangObject &value) {
  value.toString(os);
  return os;
}
std::istream &operator>>(std::istream &is, LangObject &value) {
  value.cin(is);
  return is;
}

void LangFunction::toString(std::ostream &os) const {
  os << (std::string) "< function >";
}


void LangString::toString(std::ostream &os) const { os << this->value; }
void LangString::cin(std::istream &is) { is >> this->value; }
LangBoolean *LangString::operator==(const LangObject &rhs) const {
  return new LangBoolean(this->value == ((LangString &) rhs).value);
}

void LangNumber::toString(std::ostream &os) const { os << this->value; }
void LangNumber::cin(std::istream &is) { is >> this->value; }
LangObject *LangNumber::operator-() { return new LangNumber(-this->value); }
LangObject *LangNumber::operator+() { return new LangNumber(+this->value); }

void LangBoolean::toString(std::ostream &os) const {
  os << (std::string) (this->value ? "true" : "false");
}
void LangBoolean::cin(std::istream &is) { is >> this->value; }

void LangClass::toString(std::ostream &os) const {
  os << (std::string) "< class:" + this->name + " >";
}

void LangVoid::toString(std::ostream &os) const {
  os << (std::string) "< void >";
}

void LangNil::toString(std::ostream &os) const {
  os << (std::string) "< nil >";
}
