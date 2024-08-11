#ifndef LEXI_SCANNER_H
#define LEXI_SCANNER_H

#include "../tokens/token.h"
#include <fstream>
#include <string>

class LexiScanner {
public:
  LexiScanner(std::string fileContent);
  /*void scan();*/
  Token nextToken();

private:
  int position;

  std::string fileContent;
  
  bool isDigit(char c);
  bool isLetter(char c);
  bool isOperator(char c);
  bool isSemicolon(char c);
  bool isWhitespace(char c);
  bool isCmpOperator(char c);
  bool isUpperLetter(char c);
  bool isDoubleQuotes(char c);
  bool isEOF();

  char nextChar();
  void backChar();
};

#endif // LEXI_SCANNER_H
