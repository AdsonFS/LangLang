#ifndef ERROR_H
#define ERROR_H

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

class LexicalError : public CoreError {
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
    message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

class SyntaxError : public CoreError {
private:
  std::string message;

public:
  explicit SyntaxError(const std::string &section, const std::string &token,
                       std::pair<int, int> position, std::string expected) {
    std::ostringstream oss;
    oss << this->grey << "Syntax Error: " << this->reset << "expected " << expected << ", found " << this->red << token
        << this->reset << ", on line " << position.first << ":"
        << position.second << std::endl
        << std::setw(4) << position.first << " | " << section << std::endl
        << std::setw(4) << "" << " | "
        << std::string(std::max(0, position.second - 1), ' ') << this->red
        << "^" << this->reset;
    message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

#endif // ERROR_H
