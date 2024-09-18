#ifndef ERROR_H
#define ERROR_H

#include "../file_handle/file_handle.h"
#include "../tokens/token.h"
#include <exception>
#include <iomanip>
#include <string>
#include <utility>

class CoreError : public std::exception {
protected:
  const std::string bold = "\033[1m";
  const std::string red = "\033[31m";
  const std::string reset = "\033[0m";
  const std::string grey = "\033[97m";
};
class InternalError : public CoreError {};
class ExternalError : public CoreError {};

class LangObject;
class ASTValue {
public:
  explicit ASTValue(LangObject *value) : value(value) {}
  LangObject *value;
};

class ReturnError : public InternalError {
public:
  explicit ReturnError(ASTValue *value) : value(value) {}
  ASTValue *value;
};

class LexicalError : public ExternalError {
private:
  std::string message;

public:
  explicit LexicalError(Token token, std::string error = "Unknown symbol") {
    std::ostringstream oss;

    oss << this->bold << FileHandle::fileName << ": " << this->reset
        << this->grey << "Lexical Error: " << this->red << error << this->reset
        << ", on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl
        << std::setw(4) << token.getPosition().getLine() << " | "
        << FileHandle::getLineError(token.getPosition().getPosition())
        << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, token.getPosition().getColumn() - 1), ' ')
        << this->red << "^" << this->reset;
    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

class SyntaxError : public ExternalError {
private:
  std::string message;

public:
  explicit SyntaxError(Token &token, std::string expected) {
    std::ostringstream oss;
    oss << this->bold << FileHandle::fileName << ": " << this->reset
        << this->grey << "Syntax Error: " << this->reset << "expected "
        << expected << ", found " << this->red << token.getValue()
        << this->reset << ", on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl
        << std::setw(4) << token.getPosition().getLine() << " | "
        << FileHandle::getLineError(token.getPosition().getPosition())
        << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, token.getPosition().getColumn() - 1), ' ')
        << this->red << "^" << this->reset;
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
    oss << this->bold << FileHandle::fileName << ": " << this->reset
        << this->grey << "Semantic Error: " << this->reset << message
        << ", on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl
        << std::setw(4) << token.getPosition().getLine() << " | "
        << FileHandle::getLineError(token.getPosition().getPosition())
        << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, token.getPosition().getColumn() - 1), ' ')
        << this->red << "^" << this->reset;

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
    oss << this->bold << FileHandle::fileName << ": " << this->reset
        << this->grey << "Runtime Error: " << this->reset << message
        << ", on line " << token.getPosition().getLine() << ":"
        << token.getPosition().getColumn() << std::endl;

    this->message = oss.str();
  }
  // TODO -> add the line and column to the message
  explicit RuntimeError(const std::string &message) {
    std::ostringstream oss;
    oss << this->bold << FileHandle::fileName << ": " << this->reset
        << this->grey << "Runtime Error: " << this->reset << message
        << std::endl;
    this->message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

#endif // ERROR_H
