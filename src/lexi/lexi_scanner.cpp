#include "../lexi/lexi_scanner.h"
#include <string>

LexiScanner::LexiScanner(std::string fileContent) {
  // add EOF for tests
  if (fileContent[fileContent.size() - 1] != 10)
    fileContent.push_back(10);

  this->fileContent = fileContent;
  this->position = 0;
  this->reservedWords = {"NUMBER", "STRING"};
}

Token LexiScanner::nextToken() {
  int state = 0;
  char currentChar;
  std::string tokenValue = "";
  while (1 < 2) {
    currentChar = this->nextChar();

    switch (state) {
    case 0:
      if (this->isEOF() && this->isWhitespace(currentChar))
        return Token(TokenType::TK_EOF, "EOF");
      else if (this->isWhitespace(currentChar))
        continue;
      else if (this->isOperator(currentChar))
        return Token(TokenType::TK_OPERATOR, std::string(1, currentChar));
      else if (this->isSemicolon(currentChar))
        return Token(TokenType::TK_SEMICOLON, ";");
      else if (this->isParentheses(currentChar))
        return Token(TokenType::TK_PARENTHESES, std::string(1, currentChar));
      else if (this->isAssignment(currentChar))
        return Token(TokenType::TK_ASSIGNMENT, "=");
      else if (this->isUpperLetter(currentChar))
        state = 4;
      else if (this->isDigit(currentChar))
        state = 2;
      else if (this->isLowerLetter(currentChar))
        state = 3;
      else if (this->isDoubleQuotes(currentChar)) {
        state = 1;
        continue;
      } else if (currentChar == '>' && this->peekChar() == '>') {
        this->nextChar();
        return Token(TokenType::TK_OUTPUTSTREAM, ">>");
      } else if (currentChar == '<' && this->peekChar() == '<') {
        this->nextChar();
        return Token(TokenType::TK_INPUTSTREAM, "<<");
      } else
        throw std::runtime_error("Unknown Symbol: " + tokenValue);
      break;
    case 1:
      if (this->isDoubleQuotes(currentChar)) {
        return Token(TokenType::TK_STRING, tokenValue);
      }
      break;
    case 2:
      if (this->isDigit(currentChar))
        state = 2;
      else if (this->isLowerLetter(currentChar) ||
               this->isUpperLetter(currentChar))
        throw std::runtime_error("Unknown Symbol: " + tokenValue);
      else {
        this->backChar();
        return Token(TokenType::TK_NUMBER, tokenValue);
      }
      break;
    case 3:
      if (this->isLowerLetter(currentChar) || this->isDigit(currentChar))
        state = 3;
      else {
        this->backChar();
        return Token(TokenType::TK_IDENTIFIER, tokenValue);
      }
      break;
    case 4:
      if (this->isUpperLetter(currentChar))
        state = 4;
      else if (this->reservedWords.find(tokenValue) !=
               this->reservedWords.end()) {
        this->backChar();
        return Token(TokenType::TK_RESERVED_WORD, tokenValue);
      } else
        throw std::runtime_error("Unknown Symbol: " + tokenValue);
      break;
    }
    tokenValue.push_back(currentChar);
  }
  return Token(TokenType::TK_UNKNOWN, "");
}

bool LexiScanner::isSemicolon(char c) { return c == ';'; }

bool LexiScanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool LexiScanner::isLowerLetter(char c) { return (c >= 'a' && c <= 'z'); }

bool LexiScanner::isUpperLetter(char c) { return c >= 'A' && c <= 'Z'; }

bool LexiScanner::isOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool LexiScanner::isCmpOperator(char c) {
  return c == '<' || c == '>' || c == '=' || c == '!';
}

bool LexiScanner::isWhitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0';
}

bool LexiScanner::isAssignment(char c) { return c == '='; }
bool LexiScanner::isEOF() { return this->position >= this->fileContent.size(); }

char LexiScanner::nextChar() {
  if (this->position >= this->fileContent.size())
    return '\0';
  return this->fileContent[this->position++];
}
char LexiScanner::peekChar() {
  if (this->position >= this->fileContent.size())
    return '\0';
  return this->fileContent[this->position];
}
bool LexiScanner::isParentheses(char c) { return c == '(' || c == ')'; }
bool LexiScanner::isDoubleQuotes(char c) { return c == '"'; }
void LexiScanner::backChar() {
  if (!this->isEOF())
    this->position--;
}
