#include "../../src/lexi/lexi_scanner.h"
#include <gtest/gtest.h>
#include <string>

TEST(LexiTest, lexi_string_success_test) {
  LexiScanner scanner("   \"234\"\"string\"\n\t\"test\"\"test 2\"\t\"#2-.,aZ~\"");
  std::string valueTokens[] = {"234", "string", "test", "test 2", "#2-.,aZ~"};
  Token token;
  
  for (int i = 0; i < 5; i++) {
    EXPECT_NO_THROW(token = scanner.nextToken());
    EXPECT_EQ(token.getType(), TokenType::TK_STRING);
    EXPECT_EQ(token.getValue(), valueTokens[i]);
  }
  EXPECT_NO_THROW(token = scanner.nextToken());
  EXPECT_EQ(token.getType(), TokenType::TK_EOF);
}
