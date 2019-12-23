#ifndef ELEMENT
#define ELEMENT

#include <string>

// enum ElementProperty {NAME, SYMBOL, ATOMIC_NUMBER, PERIOD, GROUP, ELECTRONEGATIVITY, RADIUS};

struct Element
{
  std::string name;
  std::string symbol;
  uint8_t atomicNumber;
  int8_t period;
  int8_t group;
  float electronegativity;
  short int radius; // Calculated Radius
};

#endif
