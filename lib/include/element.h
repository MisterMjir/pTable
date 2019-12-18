#ifndef ELEMENT
#define ELEMENT

#include <string>

enum Property {NAME, SYMBOL, ATOMIC_NUMBER, PERIOD, GROUP, ELECTRONEGATIVITY};

struct Element
{
  std::string name;
  std::string symbol;
  unsigned short int atomicNumber;
  short int period;
  short int group;
  float electronegativity;
};

#endif
