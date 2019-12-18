#include "table.h"
#include <iostream>
#include "utility.h"
#include "version.h"

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
 void Table::handleCommand(const std::string &command)
 {
   std::cout << "\n"; // Separate output from what user typed

   std::vector<std::string> args = seperateString(command, " ");
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
 * getElement
 * -------------
 * Use a query of integer or string to find an element
 * -------------
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
 * queryElement
 * ------------
 * Queries and reuturns an element, null if failed
 * ------------
 * Params
 * std::string arg | Can be integer (atomic number), element symbol (case sensitive), or element name (case insensitive)
 */
Element* Table::queryElement(const std::string &arg)
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
  std::cout << "Atomic Number: " << elm->atomicNumber << "\n";
  std::cout << "Period: " << elm->period << "\tGroup: ";

  if (elm->group > 1)
    std::cout << elm->group;
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
 * std::vector<std::string> args | Arguments param required by Method struct
 */
void Table::helpHelp(const std::vector<std::string> &args)
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
void Table::displayElement(const std::vector<std::string> &args)
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
 * Table::compareElements
 * ----------------------
 * Use 'compare help' ;)
 */
void Table::compareElements(const std::vector<std::string> &args)
{
  std::vector<std::string> subCommands;
  subCommands.push_back("all");
  subCommands.push_back("electronegativity");

  if (checkstrnocase(args.at(0), subCommands.at(0)))
  {
    std::cout << "Coming in a future update\n";
  }
  else if (checkstrnocase(args.at(0), subCommands.at(1)))
  {
    Element* elm1 = queryElement(args.at(1));
    Element* elm2 = queryElement(args.at(2));
    if (elm1 && elm2)
    {
      if (elm1->electronegativity > elm2->electronegativity)
      {
        std::cout << elm1->name << " with an electronegativity value of " << elm1->electronegativity << " is greater than\n";
        std::cout << elm2->name << " with an electronegativity value of " << elm2->electronegativity << "\n";
        std::cout << "The difference between the values is " << (elm1->electronegativity - elm2->electronegativity) << "\n";
      }
      else
      {
        std::cout << elm2->name << " with an electronegativity value of " << elm2->electronegativity << " is greater than " << elm1->name << " with an electronegativity value of " << elm1->electronegativity << "\n";
        std::cout << "The difference between the values is " << (elm2->electronegativity - elm1->electronegativity) << "\n";
      }
    }
    else
      std::cout << "Could not process the elements\n";
  }
  else
  {
    std::cout << "Could not process the command: " << args.at(0) << "\n";
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
  std::cout << "\tTwo elements:\n";
  std::cout << "\t\tA number for atomic number\n";
  std::cout << "\t\tText for the element symbol or name (symbol is case sensitive: FIRSTLETTERsecondletter)\n\n";
  std::cout << "ex. compare electronegativity H Carbon\n\n";
}

/*
 * getVersion
 * ----------
 * Prints out version
 */
static void getVersion(const std::vector<std::string> &args)
{
  std::cout << "The version is " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "\n";
}

/*
 * getVersionHelp
 * --------------
 * The help function for getVersion()
 */
static void getVersionHelp()
{
  std::cout << "Display the current version\n";
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
 * void (*method)(std::vector<std::string>) | The method that will get called
 * int argc                                 | How many arguments the method takes
 * std::string info                         | A short blurb on what the function does
 * void (*help)()                           | The method to call if the argument is 'help', can be NULL if argc is 0
 */
void Table::addMethod(const std::string &name, void (*method)(const std::vector<std::string> &), int argc, const std::string &info, void (*help)())
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
  addMethod("help", helpHelp, 0, "Displays helpful information", NULL);
  addMethod("exit", doNothing, 0, "Exit the program", NULL);
  addMethod("display", displayElement, 1, "Find an element and display useful information", displayElementHelp);
  addMethod("compare", compareElements, 3, "Compares properties of elements", compareElementsHelp);
  addMethod("version", getVersion, 0, "Displays the pTable version", getVersionHelp);
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
void Table::addElement(const std::string &name, const std::string &symbol, int atomicNumber, int period, int group, float electronegativity)
{
  Element* elm = new Element;
  elm->name = name;
  elm->symbol = symbol;
  elm->atomicNumber = atomicNumber;
  elm->period = period;
  elm->group = group;
  elm->electronegativity = electronegativity;

  table.push_back(elm);
}

/*
 * Table::addElements
 * -----------------
 * Call Table::addElement to add all the elements to Table::table
 */
void Table::addElements()
{
  addElement("Hydrogen",      "H",    1, 1,  1, 2.20);
  addElement("Helium",        "He",   2, 1, 18, 0.00);
  addElement("Lithium",       "Li",   3, 2,  1, 0.98);
  addElement("Beryllium",     "Be",   4, 2,  2, 1.57);
  addElement("Boron",         "B",    5, 2, 13, 2.04);
  addElement("Carbon",        "C",    6, 2, 14, 2.55);
  addElement("Nitrogen",      "N",    7, 2, 15, 3.04);
  addElement("Oxygen",        "O",    8, 2, 16, 3.44);
  addElement("Fluorine",      "F",    9, 2, 17, 3.98);
  addElement("Neon",          "Ne",  10, 2, 18, 0.00);
  addElement("Sodium",        "Na",  11, 3,  1, 0.93);
  addElement("Magnesium",     "Mg",  12, 3,  2, 1.31);
  addElement("Aluminium",     "Al",  13, 3, 13, 1.61);
  addElement("Silicon",       "Si",  14, 3, 14, 1.90);
  addElement("Phosphorus",    "P",   15, 3, 15, 2.19);
  addElement("Sulfur",        "S",   16, 3, 16, 2.58);
  addElement("Chlorine",      "Cl",  17, 3, 17, 3.16);
  addElement("Argon",         "Ar",  18, 3, 18, 0.00);
  addElement("Potassium",     "K",   19, 4,  1, 0.82);
  addElement("Calcium",       "Ca",  20, 4,  2, 1.00);
  addElement("Scandium",      "Sc",  21, 4,  3, 1.36);
  addElement("Titanium",      "Ti",  22, 4,  4, 1.54);
  addElement("Vanadium",      "V",   23, 4,  5, 1.63);
  addElement("Chromium",      "Cr",  24, 4,  6, 1.66);
  addElement("Manganese",     "Mn",  25, 4,  7, 1.55);
  addElement("Iron",          "Fe",  26, 4,  8, 1.83);
  addElement("Cobalt",        "Co",  27, 4,  9, 1.88);
  addElement("Nickel",        "Ni",  28, 4, 10, 1.91);
  addElement("Copper",        "Cu",  29, 4, 11, 1.90);
  addElement("Zinc",          "Zn",  30, 4, 12, 1.65);
  addElement("Gallium",       "Ga",  31, 4, 13, 1.81);
  addElement("Germanium",     "Ge",  32, 4, 14, 2.01);
  addElement("Arsenic",       "As",  33, 4, 15, 2.18);
  addElement("Selenium",      "Se",  34, 4, 16, 2.55);
  addElement("Bromine",       "Br",  35, 4, 17, 2.96);
  addElement("Krypton",       "Kr",  36, 4, 18, 3.00);
  addElement("Rubidium",      "Rb",  37, 5,  1, 0.82);
  addElement("Strontium",     "Sr",  38, 5,  2, 0.95);
  addElement("Yttrium",       "Y",   39, 5,  3, 1.22);
  addElement("Zicronium",     "Zr",  40, 5,  4, 1.33);
  addElement("Niobium",       "Nb",  41, 5,  5, 1.60);
  addElement("Molybdenum",    "Mo",  42, 5,  6, 2.16);
  addElement("Technetium",    "Tc",  43, 5,  7, 1.90);
  addElement("Ruthenium",     "Ru",  44, 5,  8, 2.20);
  addElement("Rhodium",       "Rh",  45, 5,  9, 2.28);
  addElement("Palladium",     "Pd",  46, 5, 10, 2.20);
  addElement("Silver",        "Ag",  47, 5, 11, 1.93);
  addElement("Cadmium",       "Cd",  48, 5, 12, 1.69);
  addElement("Indium",        "In",  49, 5, 13, 1.78);
  addElement("Tin",           "Sn",  50, 5, 14, 1.96);
  addElement("Antimony",      "Sb",  51, 5, 15, 2.05);
  addElement("Tellurium",     "Te",  52, 5, 16, 2.10);
  addElement("Iodine",        "I",   53, 5, 17, 2.66);
  addElement("Xenon",         "Xe",  54, 5, 18, 2.60);
  addElement("Caesium",       "Cs",  55, 6,  1, 0.79);
  addElement("Barium",        "Ba",  56, 6,  2, 0.89);
  addElement("Lanthanum",     "La",  57, 6, -1, 1.10);
  addElement("Cerium",        "Ce",  58, 6, -1, 1.12);
  addElement("Praseodymium",  "Pr",  59, 6, -1, 1.13);
  addElement("Neodymium",     "Nd",  60, 6, -1, 1.14);
  addElement("Promethium",    "Pm",  61, 6, -1, 0.00);
  addElement("Samarium",      "Sm",  62, 6, -1, 1.17);
  addElement("Europium",      "Eu",  63, 6, -1, 0.00);
  addElement("Gadolinium",    "Gd",  64, 6, -1, 1.20);
  addElement("Terbium",       "Tb",  65, 6, -1, 0.00);
  addElement("Dysprosium",    "Dy",  66, 6, -1, 1.22);
  addElement("Holmium",       "Ho",  67, 6, -1, 1.23);
  addElement("Erbium",        "Er",  68, 6, -1, 1.24);
  addElement("Thulium",       "Tm",  69, 6, -1, 1.25);
  addElement("Ytterbium",     "Yb",  70, 6, -1, 0.00);
  addElement("Lutetium",      "Lu",  71, 6, -1, 1.27);
  addElement("Hafnium",       "Hf",  72, 6,  4, 1.30);
  addElement("Tantalum",      "Ta",  73, 6,  5, 1.50);
  addElement("Tungsten",      "W",   74, 6,  6, 2.36);
  addElement("Rhenium",       "Re",  75, 6,  7, 1.90);
  addElement("Osmium",        "Os",  76, 6,  8, 2.20);
  addElement("Iridium",       "Ir",  77, 6,  9, 2.20);
  addElement("Platinum",      "Pt",  78, 6, 10, 2.28);
  addElement("Gold",          "Au",  79, 6, 11, 2.54);
  addElement("Mercury",       "Hg",  80, 6, 12, 2.00);
  addElement("Thallium",      "Tl",  81, 6, 13, 1.62);
  addElement("Lead",          "Pb",  82, 6, 14, 2.33);
  addElement("Bismuth",       "Bi",  83, 6, 15, 2.02);
  addElement("Polonium",      "Po",  84, 6, 16, 2.00);
  addElement("Astatine",      "At",  85, 6, 17, 2.20);
  addElement("Radon",         "Rn",  86, 6, 18, 0.00);
  addElement("Francium",      "Fr",  87, 7,  1, 0.70);
  addElement("Radium",        "Fa",  88, 7,  2, 0.90);
  addElement("Actinium",      "Ac",  89, 7, -2, 1.10);
  addElement("Thorium",       "Th",  90, 7, -2, 1.30);
  addElement("Protactinum",   "Pa",  91, 7, -2, 1.50);
  addElement("Uranium",       "U",   92, 7, -2, 1.38);
  addElement("Neptunium",     "Np",  93, 7, -2, 1.36);
  addElement("Plutonium",     "Pu",  94, 7, -2, 1.28);
  addElement("Americium",     "Am",  95, 7, -2, 1.30);
  addElement("Curium",        "Cm",  96, 7, -2, 1.30);
  addElement("Berkelium",     "Bk",  97, 7, -2, 1.30);
  addElement("Californium",   "Cf",  98, 7, -2, 1.30);
  addElement("Einsteinium",   "Es",  99, 7, -2, 1.30);
  addElement("Fermium",       "Fm", 100, 7, -2, 1.30);
  addElement("Mendelevium",   "Md", 101, 7, -2, 1.30);
  addElement("Nobelium",      "No", 102, 7, -2, 1.30);
  addElement("Lawrencium",    "Lr", 103, 7, -2, 0.00);
  addElement("Rutherfordium", "Rf", 104, 7,  4, 0.00);
  addElement("Dubnium",       "Db", 105, 7,  5, 0.00);
  addElement("Seaborgium",    "Sg", 106, 7,  6, 0.00);
  addElement("Bohrium",       "Bh", 107, 7,  7, 0.00);
  addElement("Hassium",       "Hs", 108, 7,  8, 0.00);
  addElement("Meitnerium",    "Mt", 109, 7,  9, 0.00);
  addElement("Darmstadtium",  "Ds", 110, 7, 10, 0.00);
  addElement("Roentgenium",   "Rg", 111, 7, 11, 0.00);
  addElement("Copernicium",   "Cn", 112, 7, 12, 0.00);
  addElement("Nihonium",      "Nh", 113, 7, 13, 0.00);
  addElement("Flerovium",     "Fl", 114, 7, 14, 0.00);
  addElement("Moscovium",     "Mc", 115, 7, 15, 0.00);
  addElement("Livermonium",   "Lv", 116, 7, 16, 0.00);
  addElement("Tennessine",    "Ts", 117, 7, 17, 0.00);
  addElement("Oganesson",     "Og", 118, 7, 18, 0.00);
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
