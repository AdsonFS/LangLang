#ifndef ERROR_H
#define ERROR_H

#include "../tokens/token.h"
#include <exception>
#include <iomanip>
#include <string>
#include <utility>

class CoreError : public std::exception {
protected:
  const std::string grey = "\033[97m";
  const std::string red = "\033[31m";
  const std::string reset = "\033[0m";
};
class InternalError : public CoreError {};
class ExternalError : public CoreError {};

class LangObject;
class ASTValue {
public:
  explicit ASTValue(LangObject *value) : value(value) {}
  LangObject *value;
};
/*typedef LangObject ASTValue;*/
class ReturnError : public InternalError {
public:
  explicit ReturnError(ASTValue *value) : value(value) {}
  ASTValue *value;
};

class LexicalError : public ExternalError {
private:
  std::string message;

public:
  explicit LexicalError(const std::string &token, int line, int column,
                        std::string error = "Unknown symbol") {
    std::ostringstream oss;
    oss << this->grey << "Lexical Error: " << this->red << error << " "
        << this->reset << "on line " << line << ":" << column << std::endl
        << std::setw(4) << line << " | " << token << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, column - 1), ' ') << this->red << "^"
        << this->reset;
    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

class SyntaxError : public ExternalError {
private:
  std::string message;

public:
  explicit SyntaxError(const std::string &section, const std::string &token,
                       std::pair<int, int> position, std::string expected) {
    std::ostringstream oss;
    oss << this->grey << "Syntax Error: " << this->reset << "expected "
        << expected << ", found " << this->red << token << this->reset
        << ", on line " << position.first << ":" << position.second << std::endl
        << std::setw(4) << position.first << " | " << section << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, position.second - 1), ' ') << this->red
        << "^" << this->reset;
    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

class SemanticError : public CoreError {
private:
  std::string message;

public:
  explicit SemanticError(const std::string &message, Token &token) {
    std::ostringstream oss;
    oss << this->grey << "Semantic Error: " << this->reset << message
        << " on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl;

    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

class RuntimeError : public CoreError {
private:
  std::string message;

public:
  explicit RuntimeError(const std::string &message, Token &token) {
    std::ostringstream oss;
    oss << this->grey << "Runtime Error: " << this->reset << message
        << " on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl;

    this->message = oss.str();
  }
  // TODO -> add the line and column to the message
  explicit RuntimeError(const std::string &message) {
    std::ostringstream oss;
    oss << this->grey << "Runtime Error: " << this->reset << message
        << std::endl;
    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

#endif // ERROR_H
