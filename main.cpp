#include <iostream>
#include <string>
#include "table.h"

int main(int argc, char* args[])
{
  Table table = Table();
  std::string command = "";

  std::cout << "==================================================================\n";
  std::cout << "\npTable by Mister Mjir\n";
  std::cout << "Find out stuff about elements. For a list of commands, type 'help'\n";
  std::cout << "Type 'exit' to quit\n\n";
  std::cout << "==================================================================\n\n";

  while (command.compare("exit") != 0)
  {
    std::getline(std::cin, command);
    table.handleCommand(command);
  }

  return 0;
}
