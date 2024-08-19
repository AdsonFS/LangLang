#include "../../../src/parser/lang_parser.h"
#include <gtest/gtest.h>

TEST(IntegerValidatorTest, valid_integer_result) {
  std::string expressions[] = {"1+2-3*0", "((3)-5)*2", "(-3)-5*2", "10%(2+1*3)-2/1",
                               "100-10/2", "((100-10)/2)", "-10%+-7", "(7%-3)- -1"};
  int results[] = {3, -4, -13, -2, 95, 45, -3, 2}; 
  for (int i = 0; i < 8; i++) {
    LexiScanner scanner(expressions[i]);
    Token token = scanner.nextToken();
    LangParser langParser(scanner, token);
    EXPECT_EQ(std::get<int>(langParser.numericExpression()->solve()), results[i]);
  }
}
