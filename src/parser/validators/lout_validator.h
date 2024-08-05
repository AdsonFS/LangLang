#ifndef LOUT_VALIDATOR_H
#define LOUT_VALIDATOR_H

class LoutValidator {
public:
  LoutValidator(void (*nextToken)());

private:
  void (*nextToken)();
};

#endif // !LOUT_VALIDATOR_H
