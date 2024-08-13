#include "../../../src/parser/validators/expression_validator.h"
#include <gtest/gtest.h>

TEST(IntegerValidatorTest, valid_integer_result) {
  std::string expressions[] = {"1+2-3*0", "((3)-5)*2", "(3)-5*2", "10%(2+1*3)-2/1",
                               "100-10/2", "((100-10)/2)"};
  int results[] = {3, -4, -7, -2, 95, 45}; 
  for (int i = 0; i < 6; i++) {
    LexiScanner scanner(expressions[i]);
    Token token = scanner.nextToken();
    ExpressionValidator expressionValidator(scanner, token);
    EXPECT_EQ(expressionValidator.validate(), results[i]);
  }
}
