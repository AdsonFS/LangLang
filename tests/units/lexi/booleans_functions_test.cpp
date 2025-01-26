#include "../../../src/lexi/lexi_scanner.h"
#include "./lexi_fixture_test.h"
#include <gtest/gtest.h>

TEST_F(LexiUnitTest, lexi_booleans_functions) {
  const std::string valid_digits = "1234567890";
  execute_validation(valid_digits, &LexiScanner::isDigit);

  const std::string valid_operators = "+-*/%";
  execute_validation(valid_operators, &LexiScanner::isOperator);

  const std::string valid_semicolons = ";";
  execute_validation(valid_semicolons, &LexiScanner::isSemicolon);

  const std::string valid_whitespaces = " \n\t\r\0";
  execute_validation(valid_whitespaces, &LexiScanner::isWhitespace);

  const std::string valid_parentheses = "()";
  execute_validation(valid_parentheses, &LexiScanner::isParentheses);

  const std::string valid_cmp_operators = "<>";
  execute_validation(valid_cmp_operators, &LexiScanner::isCmpOperator);

  const std::string valid_upper_letters = "AZ";
  execute_validation(valid_upper_letters, &LexiScanner::isUpperLetter);

  const std::string valid_lower_letters = "az";
  execute_validation(valid_lower_letters, &LexiScanner::isLowerLetter);

  const std::string valid_curly_braces = "{}";
  execute_validation(valid_curly_braces, &LexiScanner::isCurlyBraces);

  const std::string valid_double_quotes = "\"";
  execute_validation(valid_double_quotes, &LexiScanner::isDoubleQuotes);

  EXPECT_TRUE(scanner->isLogicalOperator('&', '&'));
  EXPECT_TRUE(scanner->isLogicalOperator('|', '|'));
}

TEST_F(LexiUnitTest, lexi_is_eof) {
  EXPECT_FALSE(scanner->isEOF());
  scanner->nextChar();
  EXPECT_TRUE(scanner->isEOF());
}
