#include "lexi_scanner.h"
#include "token.h"
#include <iostream>

LexiScanner::LexiScanner(std::string filename) {
  this->file.open(filename);
  if (!this->file.is_open()) {
    std::cout << "Error: could not open file " << filename << std::endl;
    exit(1);
  }
  this->fileContent = std::string((std::istreambuf_iterator<char>(this->file)),
                                  std::istreambuf_iterator<char>());
  this->position = 0;
}

Token LexiScanner::nextToken() {
  int state = 0;
  char currentChar;
  std::string tokenValue = "";
  while (1 < 2) {
    if (this->isEOF()) {
      return Token(TokenType::TK_EOF, "EOF");
    }
    currentChar = this->nextChar();

    switch (state) {
    case 0:
      if (this->isWhitespace(currentChar))
        state = 0;
      else if (this->isLetter(currentChar))
        state = 1;
      else if (this->isDigit(currentChar))
        state = 3;
      else if (this->isOperator(currentChar))
        state = 5;
      else if (this->isSemicolon(currentChar)) {
        this->backChar();
        state = 6;
      } else
        throw std::runtime_error("Unknown Symbol");
      break;
    case 1:
      if (this->isLetter(currentChar) || this->isDigit(currentChar))
        state = 1;
      else
        state = 2;
      break;
    case 2:
      this->backChar();
      return Token(TokenType::TK_IDENTIFIER, tokenValue);
    case 3:
      if (this->isDigit(currentChar))
        state = 3;
      else if (this->isWhitespace(currentChar))
        state = 4;
      else
        throw std::runtime_error("Unknown Symbol");
      break;
    case 4:
      this->backChar();
      return Token(TokenType::TK_NUMBER, tokenValue);
    case 5:
      this->backChar();
      return Token(TokenType::TK_OPERATOR, tokenValue);
    case 6:
      return Token(TokenType::TK_SEMICOLON, ";");
    }
    if (!this->isWhitespace(currentChar))
      tokenValue.push_back(currentChar);
  }
  return Token(TokenType::TK_UNKNOWN, "");
}

bool LexiScanner::isSemicolon(char c) { return c == ';'; }

bool LexiScanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool LexiScanner::isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LexiScanner::isOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

bool LexiScanner::isCmpOperator(char c) {
  return c == '<' || c == '>' || c == '=' || c == '!';
}

bool LexiScanner::isWhitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0';
}

bool LexiScanner::isEOF() { return this->position >= this->fileContent.size(); }

char LexiScanner::nextChar() {
  if (this->position >= this->fileContent.size()) {
    return '\0';
  }
  return this->fileContent[this->position++];
}

void LexiScanner::backChar() { this->position--; }
