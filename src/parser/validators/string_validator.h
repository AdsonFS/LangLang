#ifndef STRING_VALIDATOR_H
#define STRING_VALIDATOR_H

#include "../../lexi/lexi_scanner.h"


class StringValidator {
public:
  StringValidator(LexiScanner &_scanner, Token &_token);
  std::string validate();

private:
  LexiScanner &scanner;
  Token &token;

};


#endif // STRING_VALIDATOR_H
