#ifndef LEXI_SCANNER_H
#define LEXI_SCANNER_H

#include "../tokens/token.h"
#include <fstream>
#include <gtest/gtest_prod.h>
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
  
  FRIEND_TEST(LexiUnitTest, lexi_is_digit);
  FRIEND_TEST(LexiUnitTest, lexi_is_operator);
  FRIEND_TEST(LexiUnitTest, lexi_is_semicolon);
  FRIEND_TEST(LexiUnitTest, lexi_is_whitespace);
  FRIEND_TEST(LexiUnitTest, lexi_is_parentheses);
  FRIEND_TEST(LexiUnitTest, lexi_is_cmp_operator);
  FRIEND_TEST(LexiUnitTest, lexi_is_upper_letter);
  FRIEND_TEST(LexiUnitTest, lexi_is_lower_letter);
  FRIEND_TEST(LexiUnitTest, lexi_is_curly_braces);
  FRIEND_TEST(LexiUnitTest, lexi_is_double_quotes);
  FRIEND_TEST(LexiUnitTest, lexi_is_logical_operator);
  FRIEND_TEST(LexiUnitTest, lexi_is_eof);

  char nextChar();
  char peekChar();
  void backChar();

  FRIEND_TEST(LexiUnitTest, lexi_handle_char);
};

#endif // LEXI_SCANNER_H
