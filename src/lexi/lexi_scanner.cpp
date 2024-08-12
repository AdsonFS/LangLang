#include "../lexi/lexi_scanner.h"
#include <string>
/*#include <set>*/

LexiScanner::LexiScanner(std::string fileContent) {
  this->fileContent = fileContent;
  this->position = 0;
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
      else if (this->isDigit(currentChar))
        state = 2;
      else if (this->isDoubleQuotes(currentChar)) {
        state = 1;
        continue;
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
      else if (this->isLetter(currentChar))
        throw std::runtime_error("Unknown Symbol: " + tokenValue);
      else {
        this->backChar();
        return Token(TokenType::TK_NUMBER, tokenValue);
      }
      break;
    }
    tokenValue.push_back(currentChar);
  }
  return Token(TokenType::TK_UNKNOWN, "");
}

/*Token LexiScanner::nextToken() {*/
/*  int state = 0;*/
/*  char currentChar;*/
/*  std::string tokenValue = "";*/
/*  while (1 < 2) {*/
/*    if (this->isEOF()) {*/
/*      return Token(TokenType::TK_EOF, "EOF");*/
/*    }*/
/*    currentChar = this->nextChar();*/
/**/
/*    switch (state) {*/
/*    case 0:*/
/*      if (this->isWhitespace(currentChar))*/
/*        state = 0;*/
/*      else if (this->isUpperLetter(currentChar))*/
/*        state = 7;*/
/*      else if (this->isLetter(currentChar))*/
/*        state = 1;*/
/*      else if (this->isDigit(currentChar))*/
/*        state = 3;*/
/*      else if (this->isOperator(currentChar))*/
/*        state = 5;*/
/*      else if (this->isSemicolon(currentChar)) {*/
/*        this->backChar();*/
/*        state = 6;*/
/*      } else*/
/*        throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
/*      break;*/
/*    case 1:*/
/*      if (this->isLetter(currentChar) || this->isDigit(currentChar))*/
/*        state = 1;*/
/*      else {*/
/*        this->backChar();*/
/*        state = 2;*/
/*      }*/
/*      break;*/
/*    case 2:*/
/*      this->backChar();*/
/*      return Token(TokenType::TK_IDENTIFIER, tokenValue);*/
/*    case 3:*/
/*      if (this->isDigit(currentChar))*/
/*        state = 3;*/
/*      else if (this->isWhitespace(currentChar))*/
/*        state = 4;*/
/*      else*/
/*        throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
/*      break;*/
/*    case 4:*/
/*      this->backChar();*/
/*      return Token(TokenType::TK_NUMBER, tokenValue);*/
/*    case 5:*/
/*      this->backChar();*/
/*      return Token(TokenType::TK_OPERATOR, tokenValue);*/
/*    case 6:*/
/*      return Token(TokenType::TK_SEMICOLON, ";");*/
/*    case 7:*/
/*      if (this->isUpperLetter(currentChar))*/
/*        state = 7;*/
/*      else if (this->isWhitespace(currentChar))*/
/*        state = 8;*/
/*      else*/
/*        throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
/*      break;*/
/*    case 8:*/
/*      this->backChar();*/
/*      std::set<std::string> reservedWords = {"LOUT", "IF", "THEN", "ELSE",*/
/*                                             "ENDIF"};*/
/*      if (reservedWords.find(tokenValue) != reservedWords.end())*/
/*        return Token(TokenType::TK_RESERVED_WORD, tokenValue);*/
/*      throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
/*    }*/
/*    if (!this->isWhitespace(currentChar))*/
/*      tokenValue.push_back(currentChar);*/
/*  }*/
/*  return Token(TokenType::TK_UNKNOWN, "");*/
/*}*/

bool LexiScanner::isSemicolon(char c) { return c == ';'; }

bool LexiScanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool LexiScanner::isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LexiScanner::isUpperLetter(char c) { return c >= 'A' && c <= 'Z'; }

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

bool LexiScanner::isDoubleQuotes(char c) { return c == '"'; }
void LexiScanner::backChar() { if(!this->isEOF()) this->position--; }
