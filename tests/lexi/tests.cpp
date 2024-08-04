#include <gtest/gtest.h>
#include "../../src/lexi/lexi_scanner.h"
#include <string>

TEST(LexiTest, lexi_success_test) {
  std::string fileContent = " return 0 ";
  LexiScanner scanner(fileContent);
  TokenType token = TokenType::TK_UNKNOWN;
  while (token != TokenType::TK_EOF) {
    EXPECT_NO_THROW(token = scanner.nextToken().getType());
  }
}

