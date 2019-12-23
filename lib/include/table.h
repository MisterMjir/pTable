#ifndef TABLE
#define TABLE

#include <vector>
#include <string>
#include "element.h"
#include "method.h"
#include "utility.h"

class Table
{
public:
  static void init();
  static void close();

  // Member functions
  static void handleCommand(STR_PARAM command);
  template <typename T> static Element* getElement(T query, bool (*expression)(Element*, T));
  static Element* queryElement(STR_PARAM arg);

  // Print commands
  static void printElement(Element*);
  static void printElements();
  static void help(METHOD_ARGS args);
private:
  Table();
  ~Table();
  static void destroy();

  // Add Elements
  static void addElements();
  static void addElement(STR_PARAM name, STR_PARAM symbol, uint8_t atomicNumber, int8_t period, int8_t group, float electronegativity, short int radius);
  // Add Methods
  static void addMethod(STR_PARAM name, void (*method)(METHOD_ARGS), int argc, STR_PARAM info, void (*help)());
  static void addMethods();

  // Commands
  static void displayElement(METHOD_ARGS args);
  static void compareElements(METHOD_ARGS args);

  // Data Members
  static std::vector<Element*> table;
  static std::vector<Method*> methods;
};

#endif
