#include "lexi/lang_parser.h"
#include "lexi/lexi_scanner.h"
#include "lexi/token.h"
#include <iostream>

int main() {
  std::string filename = "examples/main.ll";
  LexiScanner scanner(filename);
  LangParser parser(scanner, Token(TokenType::TK_UNKNOWN, ""));
  
  try {
    parser.Parser();
    std::cout << "Syntax is correct" << std::endl;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  }

  /*Token token = Token(TokenType::TK_UNKNOWN, "");*/
  /*do {*/
  /*  std::cout << "Next token: ";*/
  /*  token = scanner.nextToken();*/
  /*  std::cout << token.getType() << " " << token.getValue() << std::endl;*/
  /*} while (token.getType() != TokenType::TK_UNKNOWN);*/

  return 0;
}
