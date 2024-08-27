#include "../lexi/lexi_scanner.h"
#include "../error/error.h"
#include <string>

LexiScanner::LexiScanner(std::string fileContent) {
  // add EOF for tests
  if (fileContent[fileContent.size() - 1] != 10)
    fileContent.push_back(10);

  this->fileContent = fileContent;
  this->position = this->column = 0;
  this->line = 1 + (fileContent[0] == '\n');
  this->reservedWords = {"NUMBER", "STRING", "FUNC", "IF", "WHILE"};
}

void LexiScanner::panicMode() {
  while(!this->isWhitespace(this->nextChar()) && !this->isEOF());
  while(this->nextToken().getType() != TokenType::TK_SEMICOLON && !this->isEOF()); 
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

    switch (state) {
    case 0:
      if (this->isEOF() && this->isWhitespace(currentChar))
        return Token(TokenType::TK_EOF, "EOF");
      else if (currentChar == '<' && this->peekChar() == '>') {
        while (currentChar != '\0' && currentChar != '\n')
          currentChar = this->nextChar();
        return Token(TokenType::TK_COMMENT, "");
      } else if (this->isWhitespace(currentChar))
        continue;
      else if (currentChar == '=' && this->peekChar() == '=') {
        this->nextChar();
        return Token(TokenType::TK_EQUALITY_OPERATOR, "==");
      } else if (currentChar == '!' && this->peekChar() == '=') {
        this->nextChar();
        return Token(TokenType::TK_EQUALITY_OPERATOR, "!=");
      }
      else if (currentChar == '>' && this->peekChar() == '>') {
        this->nextChar();
        return Token(TokenType::TK_OUTPUTSTREAM, ">>");
      } else if (currentChar == '<' && this->peekChar() == '<') {
        this->nextChar();
        return Token(TokenType::TK_INPUTSTREAM, "<<");
      } else if (this->isOperator(currentChar))
        return Token(TokenType::TK_OPERATOR, std::string(1, currentChar));
      else if (this->isSemicolon(currentChar))
        return Token(TokenType::TK_SEMICOLON, ";");
      else if (this->isParentheses(currentChar))
        return Token(TokenType::TK_PARENTHESES, std::string(1, currentChar));
      else if (this->isAssignment(currentChar))
        return Token(TokenType::TK_ASSIGNMENT, "=");
      else if (this->isCurlyBraces(currentChar))
        return Token(TokenType::TK_CURLY_BRACES, std::string(1, currentChar));
      else if (this->isCmpOperator(currentChar) && this->peekChar() != '>')
        return Token(TokenType::TK_COMPARATOR, std::string(1, currentChar));
      else if (this->isLogicalOperator(currentChar, this->peekChar()))
        return Token(TokenType::TK_LOGICAL_OPERATOR,
                     std::string(1, currentChar) +
                         std::string(1, this->nextChar()));

      else if (this->isUpperLetter(currentChar))
        state = 4;
      else if (this->isDigit(currentChar))
        state = 2;
      else if (this->isLowerLetter(currentChar))
        state = 3;
      else if (this->isDoubleQuotes(currentChar)) {
        state = 1;
        continue;
      } else
        throw LexicalError(this->getLine(), this->line, this->column);
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

        /*throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
        throw LexicalError(this->getLine(), this->line, this->column);
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
      else if (!(currentChar == '(' || this->isWhitespace(currentChar)))
        throw LexicalError(this->getLine(), this->line, this->column);
      else if (this->reservedWords.find(tokenValue) !=
               this->reservedWords.end()) {
        this->backChar();
        return Token(TokenType::TK_RESERVED_WORD, tokenValue);
      } else
        throw LexicalError(this->getLine(), this->line, this->column);
      /*throw std::runtime_error("Unknown Symbol: " + tokenValue);*/
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

bool LexiScanner::isCmpOperator(char c) { return c == '<' || c == '>'; }

bool LexiScanner::isWhitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0';
}

bool LexiScanner::isLogicalOperator(char c1, char c2) {
  return (c1 == '&' && c2 == '&') || (c1 == '|' && c2 == '|');
}

bool LexiScanner::isCurlyBraces(char c) { return c == '{' || c == '}'; }
bool LexiScanner::isAssignment(char c) { return c == '='; }
bool LexiScanner::isEOF() { return this->position >= this->fileContent.size(); }

char LexiScanner::nextChar() {
  if (this->position >= this->fileContent.size())
    return '\0';
  this->column =
      this->fileContent[this->position] == '\n' ? 0 : this->column + 1;
  this->line += this->fileContent[this->position + 1] == '\n';
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
  if (this->isEOF())
    return;
  this->line -= this->fileContent[this->position] == '\n';
  this->column =
      this->fileContent[this->position - 1] == '\n' ? 0 : this->column - 1;
  this->position--;
}

std::pair<int, int> LexiScanner::getPosition() {
  return std::make_pair(this->line, this->column);
}

std::string LexiScanner::getLine() {
  int start, end;
  start = end = this->position;
  while (start > 0 && this->fileContent[start - 1] != '\n')
    --start;
  while (this->fileContent[end + 1] != '\0' &&
         this->fileContent[end + 1] != '\n')
    ++end;
  return this->fileContent.substr(start, end - start + 1).c_str();
}
