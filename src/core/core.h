#ifndef CORE_H
#define CORE_H

#include <iomanip>
#include <sstream>
#include <string>
#include <variant>

class AST;
typedef std::variant<int, std::string, AST *> ASTValue;

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
  explicit LexicalError(const std::string &token, int line, int column) {
    std::ostringstream oss;
    oss << this->grey << "Lexical Error: " << this->red << "Unknown Symbol "
        << this->reset << "on line " << line << ":" << column << std::endl
        << std::setw(4) << line << " | " << token << std::endl << std::setw(4) << "" << " | " << std::string(column-1, ' ') << this->red << "^" << this->reset;
    message = oss.str();
  }
  virtual const char *what() const noexcept override { return message.c_str(); }
};

#endif // CORE_H
