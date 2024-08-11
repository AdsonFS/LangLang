#include "../../../src/parser/validators/string_validator.h"
#include <gtest/gtest.h>

TEST(StringValidatorTest, valid_string_result) {
  LexiScanner scanner("\"234\"+\" \"+\"string:\"\n+\t\"test\"+\"test 2\"+\t\"#2-.,aZ~\"");
  std::string value = "234 string:testtest 2#2-.,aZ~";
  Token token = scanner.nextToken();
  StringValidator stringValidator(scanner, token);
  EXPECT_EQ(stringValidator.stringExpression(), value);
}
