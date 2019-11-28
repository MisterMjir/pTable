#ifndef TABLE
#define TABLE

#include <vector>
#include <string>
#include "element.h"

class Table
{
public:
  Table();
  ~Table();
  void printElement(std::vector<std::string> args);
  void printElements();
  void handleCommand(std::string command);
private:
  void build();
  void destroy();
  void addElement(std::string name, std::string symbol, int atomicNumber, int period, int group);
  void displayHelp();
  std::vector<Element*> table;
};

#endif
