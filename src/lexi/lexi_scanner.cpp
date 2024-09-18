#include "../lexi/lexi_scanner.h"
#include "../error/error.h"
#include "../file_handle/file_handle.h"
#include <string>

LexiScanner::LexiScanner() {
  // add EOF for tests
  if (FileHandle::fileContent[FileHandle::fileContent.size() - 1] != 10)
    FileHandle::fileContent.push_back(10);

  this->position = this->column = 0;
  this->line = 1 + (FileHandle::fileContent[0] == '\n');
  this->reservedWords = {"class",  "void", "var",  "func",  "number", "string",
                         "return", "if",   "else", "while", "for"};
}

void LexiScanner::panicMode() {
  while (!this->isWhitespace(this->nextChar()) && !this->isEOF())
    ;
  while (this->nextToken().getType() != TokenType::TK_SEMICOLON &&
         !this->isEOF())
    ;
}

Token LexiScanner::nextToken() {
  Token token = this->getNextToken();
  while (token.getType() == TokenType::TK_COMMENT)
    token = this->getNextToken();
  return token;
}

Token LexiScanner::getNextToken() {
  int state = 0;
  char currentChar;
  std::string tokenValue = "";

  while (1 < 2) {
    currentChar = this->nextChar();
    int cl = this->line - (this->peekChar() == '\n');
    int cc = this->column;
    int cp = this->position;

    switch (state) {
    case 0:
      if (this->isEOF() && this->isWhitespace(currentChar))
        return Token(TokenType::TK_EOF, "EOF", Position(cl, cc, cp));
      else if (currentChar == '<' && this->peekChar() == '>') {
        while (currentChar != '\0' && currentChar != '\n')
          currentChar = this->nextChar();
        return Token(TokenType::TK_COMMENT, "", Position(cl, cc, cp));
      } else if (this->isWhitespace(currentChar))
        continue;
      else if (currentChar == '-' && this->peekChar() == '>') {
        this->nextChar();
        return Token(TokenType::TK_ARROW, "->", Position(cl, cc, cp));
      } else if (currentChar == ':' && this->peekChar() == '=') {
        this->nextChar();
        return Token(TokenType::TK_ASSIGNMENT, ":=", Position(cl, cc, cp));
      } else if (currentChar == '=' && this->peekChar() == '=') {
        this->nextChar();
        return Token(TokenType::TK_EQUALITY_OPERATOR,
                     "==", Position(cl, cc, cp));
      } else if (currentChar == '!' && this->peekChar() == '=') {
        this->nextChar();
        return Token(TokenType::TK_EQUALITY_OPERATOR,
                     "!=", Position(cl, cc, cp));
      } else if (currentChar == '>' && this->peekChar() == '>') {
        this->nextChar();
        return Token(TokenType::TK_OUTPUTSTREAM, ">>", Position(cl, cc, cp));
      } else if (currentChar == '<' && this->peekChar() == '<') {
        this->nextChar();
        return Token(TokenType::TK_INPUTSTREAM, "<<", Position(cl, cc, cp));
      } else if (this->isOperator(currentChar))
        return Token(TokenType::TK_OPERATOR, std::string(1, currentChar),
                     Position(cl, cc, cp));
      else if (currentChar == ':')
        return Token(TokenType::TK_COLON, ":", Position(cl, cc, cp));
      else if (currentChar == '.')
        return Token(TokenType::TK_DOT, ".", Position(cl, cc, cp));
      else if (this->isSemicolon(currentChar))
        return Token(TokenType::TK_SEMICOLON, ";", Position(cl, cc, cp));
      else if (this->isParentheses(currentChar))
        return Token(TokenType::TK_PARENTHESES, std::string(1, currentChar),
                     Position(cl, cc, cp));
      else if (this->isCurlyBraces(currentChar))
        return Token(TokenType::TK_CURLY_BRACES, std::string(1, currentChar),
                     Position(cl, cc, cp));
      else if (this->isCmpOperator(currentChar) && this->peekChar() != '>')
        return Token(TokenType::TK_COMPARATOR, std::string(1, currentChar),
                     Position(cl, cc, cp));
      else if (this->isLogicalOperator(currentChar, this->peekChar()))
        return Token(TokenType::TK_LOGICAL_OPERATOR,
                     std::string(1, currentChar) +
                         std::string(1, this->nextChar()),
                     Position(cl, cc, cp));

      /*else if (this->isUpperLetter(currentChar))*/
      /*state = 4;*/
      else if (this->isDigit(currentChar))
        state = 2;
      else if (this->isLowerLetter(currentChar))
        state = 3;
      else if (this->isDoubleQuotes(currentChar)) {
        state = 1;
        continue;
      } else
        throw LexicalError(Token(TokenType::TK_UNKNOWN,
                                 std::string(1, currentChar),
                                 Position(cl, cc, cp)));
      break;
    case 1:
      if (this->isDoubleQuotes(currentChar)) {
        return Token(TokenType::TK_STRING, tokenValue, Position(cl, cc, cp));
      }
      break;
    case 2:
      if (this->isDigit(currentChar))
        state = 2;
      else if (this->isLowerLetter(currentChar) ||
               this->isUpperLetter(currentChar))
        throw LexicalError(Token(TokenType::TK_UNKNOWN,
                                 tokenValue + currentChar,
                                 Position(cl, cc, cp)));
      else {
        this->backChar();
        return Token(TokenType::TK_NUMBER, tokenValue, Position(cl, cc, cp));
      }
      break;
    case 3:
      if (this->isLowerLetter(currentChar) || this->isDigit(currentChar))
        state = 3;
      else {
        this->backChar();
        if (this->reservedWords.find(tokenValue) != this->reservedWords.end())
          return Token(TokenType::TK_RESERVED_WORD, tokenValue,
                       Position(cl, cc, cp));
        return Token(TokenType::TK_IDENTIFIER, tokenValue,
                     Position(cl, cc, cp));
      }
      break;
      /*case 4:*/
      /*  if (this->isUpperLetter(currentChar))*/
      /*    state = 4;*/
      /*  else if (!(currentChar == '(' || this->isWhitespace(currentChar)))*/
      /*    throw LexicalError(this->getLine(), this->line, this->column);*/
      /*  else if (this->reservedWords.find(tokenValue) !=*/
      /*           this->reservedWords.end()) {*/
      /*    this->backChar();*/
      /*    return Token(TokenType::TK_RESERVED_WORD, tokenValue);*/
      /*  } else*/
      /*    throw LexicalError(this->getLine(), this->line, this->column);*/
      /*throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
      /*break;*/
    }
    tokenValue.push_back(currentChar);
  }
  return Token(TokenType::TK_UNKNOWN, "",
               Position(this->line, this->column, this->position));
}

bool LexiScanner::isSemicolon(char c) { return c == ';'; }

bool LexiScanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool LexiScanner::isLowerLetter(char c) { return (c >= 'a' && c <= 'z'); }

bool LexiScanner::isUpperLetter(char c) { return c >= 'A' && c <= 'Z'; }

bool LexiScanner::isOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool LexiScanner::isCmpOperator(char c) { return c == '<' || c == '>'; }

bool LexiScanner::isWhitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0';
}

bool LexiScanner::isLogicalOperator(char c1, char c2) {
  return (c1 == '&' && c2 == '&') || (c1 == '|' && c2 == '|');
}

bool LexiScanner::isCurlyBraces(char c) { return c == '{' || c == '}'; }
bool LexiScanner::isEOF() { return this->position >= FileHandle::fileSize; }

char LexiScanner::nextChar() {
  if (this->position >= FileHandle::fileSize)
    return '\0';
  this->column =
      FileHandle::fileContent[this->position] == '\n' ? 0 : this->column + 1;
  this->line += FileHandle::fileContent[this->position + 1] == '\n';
  return FileHandle::fileContent[this->position++];
}
char LexiScanner::peekChar() {
  if (this->position >= FileHandle::fileSize)
    return '\0';
  return FileHandle::fileContent[this->position];
}
bool LexiScanner::isParentheses(char c) { return c == '(' || c == ')'; }
bool LexiScanner::isDoubleQuotes(char c) { return c == '"'; }
void LexiScanner::backChar() {
  if (this->isEOF())
    return;
  this->line -= FileHandle::fileContent[this->position] == '\n';
  this->column = FileHandle::fileContent[this->position - 1] == '\n'
                     ? 0
                     : this->column - 1;
  this->position--;
}
