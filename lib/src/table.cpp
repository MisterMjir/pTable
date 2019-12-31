#include "table.h"
#include "utility.h"
using namespace pTable;
#include "version.h"
#include <iostream>

std::vector<Element*> Table::table;
std::vector<Method*> Table::methods;

void Table::init()
{
  addElements();
  addMethods();
}

void Table::close()
{
  destroy();
}

/*
 * =============================
 * PUBLIC FUNCTIONS
 * =============================
 * Functions for everyone to use
 *
 */

 /*
  * Table::handleCommand
  * --------------------
  * Recieve input and run a command
  * --------------------
  * std::string command | A command name plus all the arguments needed for it
  */
 void Table::handleCommand(STR_PARAM command)
 {
   std::cout << "\n"; // Separate output from what user typed

   VECTOR_STR args = seperateString(command, " ");
   std::string function = args.at(0);

   args.erase(args.begin());
   bool foundFunc = false;
   for (auto method : methods)
   {
     // NOTE: Make this better
     if (function.compare(method->name) == 0)
     {
       foundFunc = true;
       if (args.size() > 0)
       {
         if (args.at(0).compare("help") == 0) // Argument is help
         {
           std::cout << method->info << "\n\n";
           if (method->help != NULL)
            method->help();
         }
         else if (args.size() < method->argc) // Missing arguments
           std::cout << "Missing a parameter. For help try '" << method->name << " help'\n";
         else // Method is good to go
           method->method(args);
       }
       else if (args.size() < method->argc) // Call function normally
        std::cout << "Missing a parameter. For help try '" << method->name << " help'\n";
       else // Call normally
        method->method(args);

       break; // Function is found so no need to continue the loop
     }
   }
   if (!foundFunc)
     std::cout << "Could not process command '" << function << "'\n";

   std::cout << "\n";
 }

/*
 * Table::getElement
 * -----------------
 * Use a query of integer or string to find an element
 * -----------------
 * Params
 * T query                         | What the search term is
 * bool (*expression)(Element*, T) | The expression that will check if the element is the same as the search term
 */
template <typename T> Element* Table::getElement(T query, bool (*expression)(Element*, T))
{
  for (auto elm : table)
    if (expression(elm, query))
      return elm;

  return NULL;
}

/*
 * Table::queryElement
 * -------------------
 * Queries and reuturns an element, null if failed
 * -------------------
 * Params
 * std::string arg | Can be integer (atomic number), element symbol (case sensitive), or element name (case insensitive)
 */
Element* Table::queryElement(STR_PARAM arg)
{
  try
  {
    int atomicNumber = std::stoi(arg);
    auto expression = [] (Element* elm, int num) -> bool {return elm->atomicNumber == num;};
    return getElement<int>(atomicNumber, expression);
  }
  catch (std::invalid_argument)
  {
    // See if the argument is a symbol
    if (arg.length() <= 2)
    {
      auto expression = [] (Element* elm, std::string str) -> bool {return elm->symbol.compare(str) == 0;};
      return getElement<std::string>(arg, expression);
    }
    // Argument is the full element name
    else
    {
      auto expression = [] (Element* elm, std::string str) -> bool {return checkstrnocase(elm->name, str);};
      return getElement<std::string>(arg, expression);
    }
  }
}

/*
 * ===========================
 * PRINT COMMANDS
 * ===========================
 * Output stuff to the console
 *
 */

/*
 * Table::printElement
 * -------------------
 * Output a single element to the console
 * -------------------
 * Params
 * Element* elm | Element that you want to output
 */
void Table::printElement(Element* elm)
{
  std::cout << elm->name << " (" << elm->symbol << ")\n";
  std::cout << "Atomic Number: " << (int) elm->atomicNumber << "\n";
  std::cout << "Period: " << (int) elm->period << "\tGroup: ";

  if (elm->group > 1)
    std::cout << (int) elm->group;
  else if (elm->group == -1)
    std::cout << "Lanthanide";
  else
    std::cout << "Actinide";

  std::cout << "\nElectronegativity: " << elm->electronegativity << "\n\n";
}

/*
 * Table::printElements
 * --------------------
 * Cycle through all the elements and call Table::printElement on them
 */
void Table::printElements()
{
  for (auto elm : table)
    printElement(elm);
}

/*
 * Table::helpHelp
 * ---------------
 * Output instructions to the console, as well as a list of all commands
 * ---------------
 * Params
 * VECTOR_STR args | Arguments param required by Method struct
 */
void Table::help(PTABLE_METHOD_ARGS args)
{
  std::cout << "Run a command by using 'CommandName arguments' (case-sensitive)\nFor more information about a command run 'CommandName help'\n\n";
  std::cout << "List of commands:\n";
  for (auto method : methods)
  {
    std::cout << method->name << " | " << method->info << "\n";
  }
}

/*
 * ===============================
 * COMMANDS
 * ===============================
 * Methods that the user will call
 */

/*
 * Table::displayElement
 * ---------------------
 * Use 'display help' ;)
 */
void Table::displayElement(PTABLE_METHOD_ARGS args)
{
  Element* elm = queryElement(args.at(0));
  if (elm)
    printElement(elm);
  else
    std::cout << "Could not find element with atomic number, symbol, or name: " << args.at(0) << "\n";
}

/*
 * displayElementHelp
 * ------------------
 * The help function for Table::displayElement()
 */
static void displayElementHelp()
{
  std::cout << "This command takes one argument:\n";
  std::cout << "\tA number for atomic number\t\t\tex. 'findElement 6'\n";
  std::cout << "\tText for either element name or element symbol\tex. 'findElement H' or 'findElement Hydrogen'\n\n";
}

/*
 * compareElementNumericProperties
 * -------------------------------
 * Helper function for Table::compareElements()
 * -------------------------------
 * PARAMS
 * std::string prop | Property name
 * std::string unit | Property unit
 * std::string name1 | Name of 1st element
 * float val1 | Value of 1st element's property
 * std::string name2 | Name of 2nd element
 * float val2 | Value of 2nd element's property
 */
static void compareElementNumericProperties(STR_PARAM prop, STR_PARAM unit, STR_PARAM name1, float val1, STR_PARAM name2, float val2)
{
  if (val1 == -1 || val2 == -1)
  {
    if (val1 == -1 && val2 == -1)
      std::cout << "Both of the selected elements don't have a known " << prop << "\n";
    else if (val1 == -1)
      std::cout << name1 << " does not have a known " << prop << "\n";
    else
      std::cout << name2 << " does not have a known " << prop << "\n";
  }
  else if (val1 > val2)
  {
    std::cout << name1 << "'s " << prop << " of " << val1 << " " << unit << " is greater than\n";
    std::cout << name2 << "'s " << prop << " of " << val2 << " " << unit << "\n";
    std::cout << "The difference between the values is " << (val1 - val2) << " " << unit << "\n";
  }
  else
  {
    std::cout << name2 << "'s " << prop << " of " << val2 << " " << unit << " is greater than\n";
    std::cout << name1 << "'s " << prop << " of " << val1 << " " << unit << "\n";
    std::cout << "The difference between the values is " << (val2 - val1) << " " << unit << "\n";
  }
}

/*
 * Table::compareElements
 * ----------------------
 * Use 'compare help' ;)
 */
void Table::compareElements(PTABLE_METHOD_ARGS args)
{
  VECTOR_STR subCommands;
  subCommands.push_back("all");
  subCommands.push_back("electronegativity");
  subCommands.push_back("radius");

  Element* elm1 = queryElement(args.at(1));
  Element* elm2 = queryElement(args.at(2));
  if (elm1 && elm2)
  {
    if (checkstrnocase(args.at(0), subCommands.at(0))) // All
      std::cout << "Coming in a future update\n";
    else if (checkstrnocase(args.at(0), subCommands.at(1))) // Electronegativity
      compareElementNumericProperties("electronegativity", "", elm1->name, elm1->electronegativity, elm2->name, elm2->electronegativity);
    else if (checkstrnocase(args.at(0), subCommands.at(2)))
      compareElementNumericProperties("radius", "pm", elm1->name, elm1->radius, elm2->name, elm2->radius);
    else
      std::cout << "Could not process the command: " << args.at(0) << "\n";
  }
  else
  {
    std::cout << "Could not process the elements.\n";
  }
}

/*
 * compareElementsHelp
 * ------------------
 * The help function for Table::compareElements()
 */
static void compareElementsHelp()
{
  std::cout << "This command takes three arguments:\n";
  std::cout << "\tThe trait to compare, the options are:\n";
  std::cout << "\t\telectronegativity\n";
  std::cout << "\t\t\tTwo elements:\n";
  std::cout << "\t\t\t\tA number for atomic number\n";
  std::cout << "\t\t\t\tText for the element symbol or name (symbol is case sensitive: FIRSTLETTERsecondletter)\n\n";
  std::cout << "\t\t\tex. compare electronegativity H Carbon\n\n";
  std::cout << "\t\tradius\n";
  std::cout << "\t\t\tTwo elements:\n";
  std::cout << "\t\t\t\tSame format for electronegativity\n\n";
}

/*
 * getVersion
 * ----------
 * Prints out version
 */
static void getVersion(PTABLE_METHOD_ARGS args)
{
  std::cout << "The version is " << PTABLE_VERSION_MAJOR << "." << PTABLE_VERSION_MINOR << "." << PTABLE_VERSION_PATCH << "\n";
}

/*
 * =================================================
 * ADD METHOD METHODS
 * =================================================
 * Create methods and add them to the methods vector
 *
 */

/*
 * Table::addMethod
 * ----------------
 * Use the given parameters to add one new method to Table::methods
 * ----------------
 * Params
 * std::string name                         | The name of the method (The one the user types)
 * void (*method)(VECTOR_STR) | The method that will get called
 * int argc                                 | How many arguments the method takes
 * std::string info                         | A short blurb on what the function does
 * void (*help)()                           | The method to call if the argument is 'help', can be NULL if argc is 0
 */
void Table::addMethod(STR_PARAM name, void (*method)(PTABLE_METHOD_ARGS), int argc, STR_PARAM info, void (*help)())
{
  Method* newMethod = new Method;
  newMethod->name = name;
  newMethod->method = method;
  newMethod->argc = argc;
  newMethod->info = info;
  newMethod->help = help;
  methods.push_back(newMethod);
}

/*
 * Table::addMethods
 * -----------------
 * Call Table::addMethod to add all the methods to Table::methods
 */
void Table::addMethods()
{
  addMethod("help", help, 0, "Displays helpful information", NULL);
  addMethod("exit", doNothing, 0, "Exit the program", NULL);
  addMethod("version", getVersion, 0, "Displays the pTable version", NULL);
  addMethod("display", displayElement, 1, "Find an element and display useful information", displayElementHelp);
  addMethod("compare", compareElements, 3, "Compares properties of elements", compareElementsHelp);
}

/*
 * ================================================
 * ADD ELEMENT METHODS
 * ================================================
 * Create elements and add them to the table vector
 *
 */

/*
 * Table::addElement
 * -----------------
 * Use the given parameters to add an element to Table::table
 * -----------------
 * Params
 * std::string name   | The name of the element
 * std::string symbol | The symbol of the element
 * int atomicNumber   | The atomic number of the element
 * int period         | The period of the element
 * int group          | The group of the element
 */
void Table::addElement(STR_PARAM name, STR_PARAM symbol, uint8_t atomicNumber, int8_t period, int8_t group, float electronegativity, short int radius)
{
  Element* elm = new Element;
  elm->name = name;
  elm->symbol = symbol;
  elm->atomicNumber = atomicNumber;
  elm->period = period;
  elm->group = group;
  elm->electronegativity = electronegativity;
  elm->radius = radius;

  table.push_back(elm);
}

/*
 * Table::addElements
 * -----------------
 * Call Table::addElement to add all the elements to Table::table
 */
void Table::addElements()
{
  int numProperties = 7;
  VECTOR_STR data = readCSV("res/table.csv", 128 * numProperties);
  for (int i = 0; i < data.size(); i+= numProperties)
  {
    addElement(data[i],
               data[i + 1],
               std::stoi(data[i + 2]),
               std::stoi(data[i + 3]),
               std::stoi(data[i + 4]),
               std::stof(data[i + 5]),
               std::stoi(data[i + 6])
              );
  }
}

/*
 * ==========================================================
 * UNCATEGORIZED
 * ==========================================================
 * Doesn't exactly belong in another section so it goes here!
 */

/*
 * Table::destroy
 * --------------
 * Deletes all the elements and methods from memory and clear both vectors
 */
void Table::destroy()
{
  for (auto elm : table)
    delete elm;
  table.clear();

  for (auto method : methods)
    delete method;
  methods.clear();
}
