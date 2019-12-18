#include <iostream>
#include <string>
#include <pTable.h>

int main(int argc, char* args[])
{
  Table::init();
  std::string command = "";

  std::cout << "==================================================================\n";
  std::cout << "\npTable by Mister Mjir\n";
  std::cout << "Find out stuff about elements. For a list of commands, type 'help'\n";
  std::cout << "Type 'exit' to quit\n\n";
  std::cout << "==================================================================\n\n";

  while (command.compare("exit") != 0)
  {
    std::getline(std::cin, command);
    Table::handleCommand(command);
  }

  Table::close();
  return 0;
}
