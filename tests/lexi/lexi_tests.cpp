#include <gtest/gtest.h>
#include "lexi/lexi_scanner.h"
// include the header of file in ../src/lexi/lexi_scanner.h
// #include "lexi/lexi_scanner.h"
/*#include "lexi/lexi_canner.h"*/

LexiScanner scanner("int main() { return 0; }");

TEST(ll, test_name)
{
    EXPECT_EQ(1, 1);
}
