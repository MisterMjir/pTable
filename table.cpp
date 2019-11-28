#include "table.h"
#include <iostream>
#include <vector>
#include <cctype>

bool checkstrnocase(std::string & str1, std::string &str2);

Table::Table()
{
 build();
}

Table::~Table()
{
  destroy();
}

void Table::build()
{
  addElement("Hydrogen", "H", 1, 1, 1);
  addElement("Helium", "He", 2, 1, 18);
  addElement("Lithium", "Li", 3, 2, 1);
  addElement("Beryllium", "Be", 4, 2, 2);
  addElement("Boron", "B", 5, 2, 13);
  addElement("Carbon", "C", 6, 2, 14);
  addElement("Nitrogen", "N", 7, 2, 15);
  addElement("Oxygen", "O", 8, 2, 16);
  addElement("Fluorine", "F", 9, 2, 17);
  addElement("Neon", "Ne", 10, 2, 18);
  addElement("Sodium", "Na", 11, 3, 1);
  addElement("Magnesium", "Mg", 12, 3, 2);
  addElement("Aluminium", "Al", 13, 3, 13);
  addElement("Silicon", "Si", 14, 3, 14);
  addElement("Phosphorus", "P", 15, 3, 15);
  addElement("Sulfur", "S", 16, 3, 16);
  addElement("Chlorine", "Cl", 17, 3, 17);
  addElement("Argon", "Ar", 18, 3, 18);
  addElement("Potassium", "K", 19, 4, 1);
  addElement("Calcium", "Ca", 20, 4, 2);
  addElement("Scandium", "Sc", 21, 4, 3);
  addElement("Titanium", "Ti", 22, 4, 4);
  addElement("Vanadium", "V", 23, 4, 5);
  addElement("Chromium", "Cr", 24, 4, 6);
  addElement("Manganese", "Mn", 25, 4, 7);
  addElement("Iron", "Fe", 26, 4, 8);
  addElement("Cobalt", "Co", 27, 4, 9);
  addElement("Nickel", "Ni", 28, 4, 10);
  addElement("Copper", "Cu", 29, 4, 11);
  addElement("Zinc", "Zn", 30, 4, 12);
  addElement("Gallium", "Ga", 31, 4, 13);
  addElement("Germanium", "Ge", 32, 4, 14);
  addElement("Arsenic", "As", 33, 4, 15);
  addElement("Selenium", "Se", 34, 4, 16);
  addElement("Bromine", "Br", 35, 4, 17);
  addElement("Krypton", "Kr", 36, 4, 18);
  addElement("Rubidium", "Rb", 37, 5, 1);
  addElement("Strontium", "Sr", 38, 5, 2);
  addElement("Yttrium", "Y", 39, 5, 3);
  addElement("Zicronium", "Zr", 40, 5, 4);
  addElement("Niobium", "Nb", 41, 5, 5);
  addElement("Molybdenum", "Mo", 42, 5, 6);
  addElement("Technetium", "Tc", 43, 5, 7);
  addElement("Ruthenium", "Ru", 44, 5, 8);
  addElement("Rhodium", "Rh", 45, 5, 9);
  addElement("Palladium", "Pd", 46, 5, 10);
  addElement("Silver", "Ag", 47, 5, 11);
  addElement("Cadmium", "Cd", 48, 5, 12);
  addElement("Indium", "In", 49, 5, 13);
  addElement("Tin", "Sn", 50, 5, 14);
  addElement("Antimony", "Sb", 51, 5, 15);
  addElement("Tellurium", "Te", 52, 5, 16);
  addElement("Iodine", "I", 53, 5, 17);
  addElement("Xenon", "Xe", 54, 5, 18);
  addElement("Caesium", "Cs", 55, 6, 1);
  addElement("Barium", "Ba", 56, 6, 2);
  addElement("Lanthanum", "La", 57, 6, -1);
  addElement("Cerium", "Ce", 58, 6, -1);
  addElement("Praseodymium", "Pr", 59, 6, -1);
  addElement("Neodymium", "Nd", 60, 6, -1);
  addElement("Promethium", "Pm", 61, 6, -1);
  addElement("Samarium", "Sm", 62, 6, -1);
  addElement("Europium", "Eu", 63, 6, -1);
  addElement("Gadolinium", "Gd", 64, 6, -1);
  addElement("Terbium", "Tb", 65, 6, -1);
  addElement("Dysprosium", "Dy", 66, 6, -1);
  addElement("Holmium", "Ho", 67, 6, -1);
  addElement("Erbium", "Er", 68, 6, -1);
  addElement("Thulium", "Tm", 69, 6, -1);
  addElement("Ytterbium", "Yb", 70, 6, -1);
  addElement("Lutetium", "Lu", 71, 6, -1);
  addElement("Hafnium", "Hf", 72, 6, 4);
  addElement("Tantalum", "Ta", 73, 6, 5);
  addElement("Tungsten", "W", 74, 6, 6);
  addElement("Rhenium", "Re", 75, 6, 7);
  addElement("Osmium", "Os", 76, 6, 8);
  addElement("Iridium", "Ir", 77, 6, 9);
  addElement("Platinum", "Pt", 78, 6, 10);
  addElement("Gold", "Au", 79, 6, 11);
  addElement("Mercury", "Hg", 80, 6, 12);
  addElement("Thallium", "Tl", 81, 6, 13);
  addElement("Lead", "Pb", 82, 6, 14);
  addElement("Bismuth", "Bi", 83, 6, 15);
  addElement("Polonium", "Po", 84, 6, 16);
  addElement("Astatine", "At", 85, 6, 17);
  addElement("Radon", "Rn", 86, 6, 18);
  addElement("Francium", "Fr", 87, 7, 1);
  addElement("Radium", "Fa", 88, 7, 2);
  addElement("Actinium", "Ac", 89, 7, -2);
  addElement("Thorium", "Th", 90, 7, -2);
  addElement("Protactinum", "Pa", 91, 7, -2);
  addElement("Uranium", "U", 92, 7, -2);
  addElement("Neptunium", "Np", 93, 7, -2);
  addElement("Plutonium", "Pu", 94, 7, -2);
  addElement("Americium", "Am", 95, 7, -2);
  addElement("Curium", "Cm", 96, 7, -2);
  addElement("Berkelium", "Bk", 97, 7, -2);
  addElement("Californium", "Cf", 98, 7, -2);
  addElement("Einsteinium", "Es", 99, 7, -2);
  addElement("Fermium", "Fm", 100, 7, -2);
  addElement("Mendelevium", "Md", 101, 7, -2);
  addElement("Nobelium", "No", 102, 7, -2);
  addElement("Lawrencium", "Lr", 103, 7, -2);
  addElement("Rutherfordium", "Rf", 104, 7, 4);
  addElement("Dubnium", "Db", 105, 7, 5);
  addElement("Seaborgium", "Sg", 106, 7, 6);
  addElement("Bohrium", "Bh", 107, 7, 7);
  addElement("Hassium", "Hs", 108, 7, 8);
  addElement("Meitnerium", "Mt", 109, 7, 9);
  addElement("Darmstadtium", "Ds", 110, 7, 10);
  addElement("Roentgenium", "Rg", 111, 7, 11);
  addElement("Copernicium", "Cn", 112, 7, 12);
  addElement("Nihonium", "Nh", 113, 7, 13);
  addElement("Flerovium", "Fl", 114, 7, 14);
  addElement("Moscovium", "Mc", 115, 7, 15);
  addElement("Livermonium", "Lv", 116, 7, 16);
  addElement("Tennessine", "Ts", 117, 7, 17);
  addElement("Oganesson", "Og", 118, 7, 18);
}

void Table::destroy()
{
  for (auto elm : table)
    delete elm;
  table.clear();
}

void Table::addElement(std::string name, std::string symbol, int atomicNumber, int period, int group)
{
  Element* elm = new Element;
  elm->name = name;
  elm->symbol = symbol;
  elm->atomicNumber = atomicNumber;
  elm->period = period;
  elm->group = group;

  table.push_back(elm);
}

void Table::printElement(std::vector<std::string> args)
{
  if (args.size() < 1)
  {
    std::cout << "Missing a parameter. For help try 'findElement help'\n";
  }
  else
  {
    if (args.at(0).compare("help") == 0)
    {
      std::cout << "Find an element and display use information\n\n";
      std::cout << "This command takes one argument:\n";
      std::cout << "\tA number for atomic number\t\t\tex. 'findElement 6'\n";
      std::cout << "\tText for either element name or element symbol\tex. 'findElement H' or 'findElement Hydrogen'\n\n";
    }
    else
    {
      try
      {
        int atomicNumber = std::stoi(args.at(0));
        bool foundElm = false;
        for (auto elm: table)
        {
          if (elm->atomicNumber == atomicNumber)
          {
            foundElm = true;
            std::cout << elm->name << " (" << elm->symbol << ")\n";
            std::cout << "Atomic Number: " << elm->atomicNumber << "\n";
            std::cout << "Period: " << elm->period << "\tGroup: " << elm->group << "\n\n";
          }
        }
        if (!foundElm)
        {
          std::cout << "Could not find element with atomic number " << atomicNumber << "\n";
        }
      }
      catch (std::invalid_argument)
      {
        std::string query = args.at(0);
        if (query.length() <= 2)
        {
          bool foundElm = false;
          for (auto elm: table)
          {
            if (elm->symbol.compare(query) == 0)
            {
              foundElm = true;
              std::cout << elm->name << " (" << elm->symbol << ")\n";
              std::cout << "Atomic Number: " << elm->atomicNumber << "\n";
              std::cout << "Period: " << elm->period << "\tGroup: " << elm->group << "\n\n";
            }
          }
          if (!foundElm)
          {
            std::cout << "Could not find element with symbol " << query << "\n";
          }
        }
        else
        {
          bool foundElm = false;
          for (auto elm: table)
          {
            if (checkstrnocase(elm->name, query))
            {
              foundElm = true;
              std::cout << elm->name << " (" << elm->symbol << ")\n";
              std::cout << "Atomic Number: " << elm->atomicNumber << "\n";
              std::cout << "Period: " << elm->period << "\tGroup: " << elm->group << "\n\n";
            }
          }
          if (!foundElm)
          {
            std::cout << "Could not find element with name " << query << "\n";
          }
        }
      }
    }
  }
}

void Table::printElements()
{
  for (auto elm : table)
  {
    std::cout << elm->name << " (" << elm->symbol << ")\n";
    std::cout << "Atomic Number: " << elm->atomicNumber << "\n";
    std::cout << "Period: " << elm->period << "\tGroup: " << elm->group << "\n\n";
  }
}

void Table::handleCommand(std::string command)
{
  std::cout << "\n"; // Separate output from what user typed

  std::vector<std::string> args;
  std::string arg = "";
  std::string function = "";

  // Get the args
  for (auto letter : command)
  {
    if (letter == ' ')
    {
      args.push_back(arg);
      arg = "";
    }
    else
    {
      arg += letter;
    }
  }
  args.push_back(arg);

  // Figure out the function
  function = args.at(0);
  args.erase(args.begin());
  if (function.compare("help") == 0)
  {
    displayHelp();
  }
  else if (function.compare("exit") == 0)
  {
    return;
  }
  else if (function.compare("findElement") == 0)
  {
    printElement(args);
  }
  else
  {
    std::cout << "Could not process command '" << function << "'\n";
  }
  std::cout << "\n";
}

void Table::displayHelp()
{
  std::cout << "Run a command by using 'CommandName arguments'\nFor more information about a command run 'CommandName help'\n\n";
  std::cout << "List of commands:\n";
  std::cout << "help | Displays helpful information\n";
  std::cout << "exit | Closes the application\n";
  std::cout << "findElement | Find an element and display useful information\n";
}

// Copied this over from https://thispointer.com/c-case-insensitive-string-comparison-using-stl-c11-boost-library/
bool checkstrnocase(std::string & str1, std::string &str2)
{
	return ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), [](char & c1, char & c2){
							return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
								}));
}
