#ifndef ELEMENT
#define ELEMENT

#include <string>

enum Property {NAME, SYMBOL, ATOMIC_NUMBER, PERIOD, GROUP};

struct Element
{
  std::string name;
  std::string symbol;
  int atomicNumber;
  int period;
  int group;
};

#endif
