#include "../../../src/file_handle/file_handle.h"
#include "../../../src/lexi/lexi_scanner.h"
#include <gtest/gtest.h>

struct LexiUnitTest : public ::testing::Test {
public:
  LexiScanner *scanner;
  LexiUnitTest() {
    FileHandle::setFileHandle("LexiUnitTest", "-");
    scanner = new LexiScanner();
    std::cout << "LexiUnitTest\n";
  }

  void set_file_content(std::string content) {
    content.push_back(10);
    FileHandle::fileContent = content;
    FileHandle::fileSize = content.size();
  }

  bool match(Token a, Token b) {
    if (a.getType() != b.getType())
      return false;
    return a.getValue() == b.getValue();
  }

  std::string get_invalid_chars(std::string valid_chars) {
    std::string invalid_chars;
    for (char c : characters)
      if (valid_chars.find(c) == std::string::npos)
        invalid_chars.push_back(c);
    return invalid_chars;
  }

  void execute_validation(std::string valid_chars,
                          bool (LexiScanner::*func)(char)) {
    const std::string invalid_chars = get_invalid_chars(valid_chars);
    for (char c : valid_chars)
      EXPECT_TRUE((*scanner.*func)(c));
    for (char c : invalid_chars)
      EXPECT_FALSE((*scanner.*func)(c));
  }

  ~LexiUnitTest() { delete scanner; }

private:
  std::string characters = "123456789_aAzZ\"!@#$^&{()}|| \n\t\r\0 +-*/%";
};
