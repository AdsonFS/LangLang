#include "../../../src/lexi/lexi_scanner.h"
#include "./lexi_fixture.h"
#include <gtest/gtest.h>

TEST_F(LexiUnitTest, lexi_is_digit) {
  const std::string valid_digits = "1234567890";
  execute_validation(valid_digits, &LexiScanner::isDigit);
}

TEST_F(LexiUnitTest, lexi_is_operator) {
  const std::string valid_operators = "+-*/%";
  execute_validation(valid_operators, &LexiScanner::isOperator);
}

TEST_F(LexiUnitTest, lexi_is_semicolon) {
  const std::string valid_semicolons = ";";
  execute_validation(valid_semicolons, &LexiScanner::isSemicolon);
}

TEST_F(LexiUnitTest, lexi_is_whitespace) {
  const std::string valid_whitespaces = " \n\t\r\0";
  execute_validation(valid_whitespaces, &LexiScanner::isWhitespace);
}

TEST_F(LexiUnitTest, lexi_is_parentheses) {
  const std::string valid_parentheses = "()";
  execute_validation(valid_parentheses, &LexiScanner::isParentheses);
}

TEST_F(LexiUnitTest, lexi_is_cmp_operator) {
  const std::string valid_cmp_operators = "<>";
  execute_validation(valid_cmp_operators, &LexiScanner::isCmpOperator);
}

TEST_F(LexiUnitTest, lexi_is_upper_letter) {
  const std::string valid_upper_letters = "AZ";
  execute_validation(valid_upper_letters, &LexiScanner::isUpperLetter);
}

TEST_F(LexiUnitTest, lexi_is_lower_letter) {
  const std::string valid_lower_letters = "az";
  execute_validation(valid_lower_letters, &LexiScanner::isLowerLetter);
}

TEST_F(LexiUnitTest, lexi_is_curly_braces) {
  const std::string valid_curly_braces = "{}";
  execute_validation(valid_curly_braces, &LexiScanner::isCurlyBraces);
}

TEST_F(LexiUnitTest, lexi_is_double_quotes) {
  const std::string valid_double_quotes = "\"";
  execute_validation(valid_double_quotes, &LexiScanner::isDoubleQuotes);
}

TEST_F(LexiUnitTest, lexi_is_logical_operator) {
  EXPECT_TRUE(scanner->isLogicalOperator('&', '&'));
  EXPECT_TRUE(scanner->isLogicalOperator('|', '|'));
}

TEST_F(LexiUnitTest, lexi_is_eof) {
  EXPECT_FALSE(scanner->isEOF());
  scanner->nextChar();
  EXPECT_TRUE(scanner->isEOF());
}
