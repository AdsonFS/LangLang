#include "lexi/lexi_scanner.h"
#include "parser/lang_parser.h"
#include "tokens/token.h"
#include <iostream>

void parser(LexiScanner &scanner, Token &token) {
  LangParser parser(scanner, token);
  try {
    parser.parser();
    std::cout << "Success!!! :)\n\n";
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  }
}

void tokens(LexiScanner &scanner) {
  Token token = Token(TokenType::TK_UNKNOWN, "");
  do {
    token = scanner.nextToken();
    std::cout << token.getType() << " " << token.getValue() << std::endl;
  } while (token.getType() != TokenType::TK_UNKNOWN &&
           token.getType() != TokenType::TK_EOF);
}

int main(int argc, char **argv) {
  std::string filename;
  if (argc > 1)
    filename = argv[1];
  else {
    filename = "examples/main.ll";
    /*std::cout << "file not found\nPlease provide\n";*/
  }

  std::ifstream file;
  file.open(filename);
  if (!file.is_open()) {
    std::cout << "Error: could not open file " << filename << std::endl;
    throw std::runtime_error("Error: could not read file " + filename);
  }
  std::string fileContent = std::string((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());
  LexiScanner scanner(fileContent);
  Token token = Token(TokenType::TK_UNKNOWN, "");
  parser(scanner, token);
  /*tokens(scanner);*/

  return 0;
}
