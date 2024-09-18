#include "ast/interpreter_visitor.h"
#include "ast/printer_visitor.h"
#include "ast/semantic_visitor.h"
#include "core/core.h"
#include "error/error.h"
#include "file_handle/file_handle.h"
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

    } catch (std::runtime_error &e) {
      std::cout << e.what() << std::endl;
      exit(1);
    }
    break;
  }
  if (!hasErrors) {
    PrinterVisitor printer;
    SemanticVisitor semantic;
    InterpreterVisitor interpreter;
    try {
      ast->accept(semantic);

      ast->accept(printer);
      interpreter.setJumpTable(SemanticVisitor::getJumpTable());
      ast->accept(interpreter);
    } catch (CoreError &e) {
      /*return 1;*/
      std::cout << e.what() << std::endl;
    }
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
  FileHandle::fileName = filename;
  FileHandle::fileContent = fileContent;
  FileHandle::fileSize = fileContent.size();

  LexiScanner scanner = LexiScanner();
  Token token = Token(TokenType::TK_UNKNOWN, "");
  return parser(scanner, token);
  /*tokens(scanner);*/
}
