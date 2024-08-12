#include "../../src/lexi/lexi_scanner.h"
#include <gtest/gtest.h>
#include <string>

TEST(LexiTest, lexi_integer_valid_tokens) {
  LexiScanner scanner("123 90 111 0 1234567890");
  std::string valueTokens[] = {"123", "90", "111", "0", "1234567890"};
  Token token;
  for (int i = 0; i < 5; i++) {
    EXPECT_NO_THROW(token = scanner.nextToken());
    EXPECT_EQ(token.getType(), TokenType::TK_NUMBER);
    EXPECT_EQ(token.getValue(), valueTokens[i]);
  }
  EXPECT_NO_THROW(token = scanner.nextToken());
  EXPECT_EQ(token.getType(), TokenType::TK_EOF);
}

TEST(LexiTest, lexi_integer_invalid_tokens) {
  std::string valueTokens[] = {"123a", "_90", "#111", "0s", "123a4567890"};
  Token token;
  for (int i = 0; i < 5; i++) {
    LexiScanner scanner(valueTokens[i]);
    EXPECT_THROW(token = scanner.nextToken(), std::runtime_error);
  }
}
