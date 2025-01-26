#include "../../../src/lexi/lexi_scanner.h"
#include "./lexi_fixture_test.h"
#include <gtest/gtest.h>

TEST_F(LexiUnitTest, lexi_handle_char) {
  set_file_content("ab23-\n");
  EXPECT_EQ(scanner->nextChar(), 'a');
  EXPECT_EQ(scanner->peekChar(), 'b');
  EXPECT_EQ(scanner->peekChar(), 'b');
  EXPECT_EQ(scanner->nextChar(), 'b');
  EXPECT_EQ(scanner->nextChar(), '2');

  scanner->backChar();
  EXPECT_EQ(scanner->nextChar(), '2');
  EXPECT_EQ(scanner->peekChar(), '3');
  EXPECT_EQ(scanner->nextChar(), '3');
  EXPECT_EQ(scanner->nextChar(), '-');
  EXPECT_EQ(scanner->nextChar(), '\n');

  scanner->backChar();
  EXPECT_EQ(scanner->nextChar(), '\n');

  EXPECT_EQ(scanner->nextChar(), '\n'); // \n added by LexiScanner constructor for tests
  EXPECT_EQ(scanner->nextChar(), '\0');
  EXPECT_TRUE(scanner->isEOF());
}
