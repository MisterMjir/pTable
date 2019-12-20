#ifndef TABLE
#define TABLE

#include <vector>
#include <string>
#include "element.h"
#include "method.h"

class Table
{
public:
  static void init();
  static void close();

  // Member functions
  static void handleCommand(const std::string &command);
  template <typename T> static Element* getElement(T query, bool (*expression)(Element*, T));
  static Element* queryElement(const std::string &arg);

  // Print commands
  static void printElement(Element*);
  static void printElements();
  static void helpHelp(const std::vector<std::string> &args);
private:
  Table();
  ~Table();
  static void destroy();

  // Add Elements
  static void addElements();
  static void addElement(const std::string &name, const std::string &symbol, int atomicNumber, int period, int group, float electronegativity, int radius);
  // Add Methods
  static void addMethod(const std::string &name, void (*method)(const std::vector<std::string> &), int argc, const std::string &info, void (*help)());
  static void addMethods();

  // Commands
  static void displayElement(const std::vector<std::string> &args);
  static void compareElements(const std::vector<std::string> &args);

  // Data Members
  static std::vector<Element*> table;
  static std::vector<Method*> methods;
};

#endif
