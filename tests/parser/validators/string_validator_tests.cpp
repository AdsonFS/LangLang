#include "../../../src/parser/validators/expression_validator.h"
#include <gtest/gtest.h>

TEST(IntegerValidatorTest, valid_integer_result) {
  LexiScanner scanner("1 + 2 + 3+4+5");
  int result = 15;
  Token token = scanner.nextToken();
  ExpressionValidator expressionValidator(scanner, token);
  EXPECT_EQ(expressionValidator.validate(), result);
}
