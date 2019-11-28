#ifndef TABLE
#define TABLE

#include <vector>
#include <string>
#include "element.h"
#include "method.h"

class Table
{
public:
  static void init(); // Initialize Table
  static void close(); // Destroy Table

  static void printElement(Element*);
  static void printElements();
  static void handleCommand(std::string command);
  static void helpHelp(std::vector<std::string> args);
  template <typename T> static void searchElement(T query, bool (*expression)(Element*, T), void (*error)(T));
private:
  Table();
  ~Table();
  static void destroy();
  static void addElements();
  static void addElement(std::string name, std::string symbol, int atomicNumber, int period, int group);
  static void addMethod(std::string name, void (*method)(std::vector<std::string>), int argc, std::string info, void (*help)());
  static void addMethods();
  static void displayElement(std::vector<std::string> args);

  static std::vector<Element*> table;
  static std::vector<Method*> methods;
};

#endif
