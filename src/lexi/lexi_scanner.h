#ifndef LEXI_SCANNER_H
#define LEXI_SCANNER_H

#include "../tokens/token.h"
#include <fstream>
#include <set>

class LexiScanner {
public:
  LexiScanner();
  Token nextToken();
  
  void panicMode();
private:
  int line;
  int column;
  int position;
  std::set<std::string> reservedWords;

  Token getNextToken();
  
  bool isDigit(char c);
  bool isOperator(char c);
  bool isSemicolon(char c);
  bool isWhitespace(char c);
  bool isParentheses(char c);
  bool isCmpOperator(char c);
  bool isUpperLetter(char c);
  bool isLowerLetter(char c);
  bool isCurlyBraces(char c);
  bool isDoubleQuotes(char c);
  bool isLogicalOperator(char c1, char c2);
  bool isEOF();

  char nextChar();
  char peekChar();
  void backChar();
};

#endif // LEXI_SCANNER_H
