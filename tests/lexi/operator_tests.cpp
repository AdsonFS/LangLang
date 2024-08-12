#include "../../src/lexi/lexi_scanner.h"
#include <gtest/gtest.h>
#include <string>

TEST(LexiTest, lexi_operator_valid_tokens) {
  LexiScanner scanner("+ - * / %-*");
  std::string valueTokens[] = {"+", "-", "*", "/", "%", "-", "*"};
  Token token;
  
  for (int i = 0; i < 7; i++) {
    EXPECT_NO_THROW(token = scanner.nextToken());
    EXPECT_EQ(token.getType(), TokenType::TK_OPERATOR);
    EXPECT_EQ(token.getValue(), valueTokens[i]);
  }
  EXPECT_NO_THROW(token = scanner.nextToken());
  EXPECT_EQ(token.getType(), TokenType::TK_EOF);
}


TEST(LexiTest, lexi_parentheses_valid_tokens) {
  LexiScanner scanner("() (( )) (())");
  std::string valueTokens[] = {"(", ")", "(", "(", ")", ")", "(", "(", ")", ")"};
  Token token;
  
  for (int i = 0; i < 10; i++) {
    EXPECT_NO_THROW(token = scanner.nextToken());
    EXPECT_EQ(token.getType(), TokenType::TK_PARENTHESES);
    EXPECT_EQ(token.getValue(), valueTokens[i]);
  }
  EXPECT_NO_THROW(token = scanner.nextToken());
  EXPECT_EQ(token.getType(), TokenType::TK_EOF);
}
