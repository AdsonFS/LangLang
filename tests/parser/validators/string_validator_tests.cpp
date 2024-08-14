#include "../../../src/parser/lang_parser.h"
#include <gtest/gtest.h>

TEST(StringValidatorTest, valid_string_result) {
  LexiScanner scanner("\"234\"+\" \"+\"string:\"\n+\t\"test\"+\"test 2\"+\t\"#2-.,aZ~\"");
  std::string value = "234 string:testtest 2#2-.,aZ~";
  Token token = scanner.nextToken();
  LangParser langParser(scanner, token);
  EXPECT_EQ(std::get<std::string>(langParser.stringExpression()->solve()), value);
}
