#ifndef LEXI_SCANNER_H
#define LEXI_SCANNER_H

#include "../tokens/token.h"
#include <fstream>
#include <set>

class LexiScanner {
public:
  LexiScanner(std::string fileContent);
  Token nextToken();
  
  void panicMode();
  std::string getLine();
  std::pair<int, int> getPosition();
private:
  int line;
  int column;
  int position;
  std::string fileContent;
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
