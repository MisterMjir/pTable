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
  static void handleCommand(std::string command);
  template <typename T> static Element* getElement(T query, bool (*expression)(Element*, T));

  // Print commands
  static void printElement(Element*);
  static void printElements();
  static void helpHelp(std::vector<std::string> args);
private:
  Table();
  ~Table();
  static void destroy();
  
  // Add Elements
  static void addElements();
  static void addElement(std::string name, std::string symbol, int atomicNumber, int period, int group);
  // Add Methods
  static void addMethod(std::string name, void (*method)(std::vector<std::string>), int argc, std::string info, void (*help)());
  static void addMethods();

  // Commands
  static void displayElement(std::vector<std::string> args);

  // Data Members
  static std::vector<Element*> table;
  static std::vector<Method*> methods;
};

#endif
