#include "lexi/lexi_scanner.h"
#include "parser/lang_parser.h"
#include <iostream>

int main(int argc, char **argv) {
  std::string filename;
  if (argc > 1)
    filename = argv[1];
  else {
    std::cout << "file not found\nPlease provide\n";
    return 1;
  }

  std::ifstream file;
  file.open(filename);
  if (!file.is_open()) {
    std::cout << "Error: could not open file " << filename << std::endl;
    throw std::runtime_error("Error: could not open file " + filename);
  }
  std::string fileContent = std::string((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());
  LexiScanner scanner(fileContent);

  /*LangParser parser(scanner, Token(TokenType::TK_UNKNOWN, ""));*/
  /*try {*/
  /*  parser.Parser();*/
  /*  std::cout << "Syntax is correct" << std::endl;*/
  /*} catch (std::runtime_error &e) {*/
  /*  std::cout << e.what() << std::endl;*/
  /*}*/

  Token token = Token(TokenType::TK_UNKNOWN, "");
  do {
    token = scanner.nextToken();
    std::cout << token.getType() << " " << token.getValue() << std::endl;
  } while (token.getType() != TokenType::TK_UNKNOWN && token.getType() != TokenType::TK_EOF);

  return 0;
}
