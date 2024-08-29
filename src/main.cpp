#include "ast/interpreter_visitor.h"
#include "ast/printer_visitor.h"
#include "core/core.h"
#include "error/error.h"
#include "lexi/lexi_scanner.h"
#include "parser/lang_parser.h"
#include "tokens/token.h"
#include <iostream>

int parser(LexiScanner &scanner, Token &token) {
  LangParser parser(scanner, token);
  bool hasErrors = false;
  AST *ast = nullptr;
  while (1 < 2) {
    try {
      ast = parser.parser();
    } catch (CoreError &e) {
      hasErrors = true;
      std::cout << e.what() << std::endl << std::endl;
      scanner.panicMode();
      continue;
      /*return 1;*/
    } catch (std::runtime_error &e) {
      std::cout << e.what() << std::endl;
      exit(1);
    }
    break;
  }
  if (!hasErrors) {
    PrinterVisitor printer;
    /*ast->accept(printer);*/

    InterpreterVisitor interpreter;
    ast->accept(interpreter);
  }
  return 0;
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
  return parser(scanner, token);
  /*tokens(scanner);*/
}
