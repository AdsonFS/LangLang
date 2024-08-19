#ifndef LEXI_SCANNER_H
#define LEXI_SCANNER_H

#include "../tokens/token.h"
#include <fstream>
#include <set>

class LexiScanner {
public:
  LexiScanner(std::string fileContent);
  Token nextToken();

  std::set<std::string> reservedWords;
private:
  int position;

  std::string fileContent;
  
  bool isDigit(char c);
  bool isOperator(char c);
  bool isSemicolon(char c);
  bool isWhitespace(char c);
  bool isAssignment(char c);
  bool isParentheses(char c);
  bool isCmpOperator(char c);
  bool isUpperLetter(char c);
  bool isLowerLetter(char c);
  bool isCurlyBraces(char c);
  bool isDoubleQuotes(char c);
  bool isEOF();

  char nextChar();
  char peekChar();
  void backChar();
};

#endif // LEXI_SCANNER_H
