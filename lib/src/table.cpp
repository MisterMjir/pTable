#include "table.h"
#include <iostream>
#include "utility.h"
using namespace pTable;
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
static void compareElementNumericProperties(const std::string &prop, const std::string &unit, const std::string &name1, float val1, const std::string &name2, float val2)
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
    std::cout << val2 << "'s " << prop << " value of " << val2 << " " << unit << " is greater than\n";
    std::cout << val1 << "'s " << prop << " value of " << val1 << " " << unit << "\n";
    std::cout << "The difference between the values is " << (val2 - val1) << " " << unit << "\n";
  }
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
static void getVersion(const std::vector<std::string> &args)
{
  std::cout << "The version is " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "\n";
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
void Table::addElement(const std::string &name, const std::string &symbol, int atomicNumber, int period, int group, float electronegativity, int radius)
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
  addElement("Hydrogen",      "H",    1, 1,  1, 2.20,  53);
  addElement("Helium",        "He",   2, 1, 18,   -1,  31);
  addElement("Lithium",       "Li",   3, 2,  1, 0.98, 167);
  addElement("Beryllium",     "Be",   4, 2,  2, 1.57, 112);
  addElement("Boron",         "B",    5, 2, 13, 2.04,  87);
  addElement("Carbon",        "C",    6, 2, 14, 2.55,  67);
  addElement("Nitrogen",      "N",    7, 2, 15, 3.04,  56);
  addElement("Oxygen",        "O",    8, 2, 16, 3.44,  48);
  addElement("Fluorine",      "F",    9, 2, 17, 3.98,  42);
  addElement("Neon",          "Ne",  10, 2, 18,   -1,  38);
  addElement("Sodium",        "Na",  11, 3,  1, 0.93, 190);
  addElement("Magnesium",     "Mg",  12, 3,  2, 1.31, 145);
  addElement("Aluminium",     "Al",  13, 3, 13, 1.61, 118);
  addElement("Silicon",       "Si",  14, 3, 14, 1.90, 111);
  addElement("Phosphorus",    "P",   15, 3, 15, 2.19,  98);
  addElement("Sulfur",        "S",   16, 3, 16, 2.58,  88);
  addElement("Chlorine",      "Cl",  17, 3, 17, 3.16,  79);
  addElement("Argon",         "Ar",  18, 3, 18,   -1,  71);
  addElement("Potassium",     "K",   19, 4,  1, 0.82, 243);
  addElement("Calcium",       "Ca",  20, 4,  2, 1.00, 194);
  addElement("Scandium",      "Sc",  21, 4,  3, 1.36, 184);
  addElement("Titanium",      "Ti",  22, 4,  4, 1.54, 176);
  addElement("Vanadium",      "V",   23, 4,  5, 1.63, 171);
  addElement("Chromium",      "Cr",  24, 4,  6, 1.66, 166);
  addElement("Manganese",     "Mn",  25, 4,  7, 1.55, 161);
  addElement("Iron",          "Fe",  26, 4,  8, 1.83, 156);
  addElement("Cobalt",        "Co",  27, 4,  9, 1.88, 152);
  addElement("Nickel",        "Ni",  28, 4, 10, 1.91, 149);
  addElement("Copper",        "Cu",  29, 4, 11, 1.90, 145);
  addElement("Zinc",          "Zn",  30, 4, 12, 1.65, 142);
  addElement("Gallium",       "Ga",  31, 4, 13, 1.81, 136);
  addElement("Germanium",     "Ge",  32, 4, 14, 2.01, 125);
  addElement("Arsenic",       "As",  33, 4, 15, 2.18, 114);
  addElement("Selenium",      "Se",  34, 4, 16, 2.55, 103);
  addElement("Bromine",       "Br",  35, 4, 17, 2.96,  94);
  addElement("Krypton",       "Kr",  36, 4, 18, 3.00,  88);
  addElement("Rubidium",      "Rb",  37, 5,  1, 0.82, 265);
  addElement("Strontium",     "Sr",  38, 5,  2, 0.95, 219);
  addElement("Yttrium",       "Y",   39, 5,  3, 1.22, 212);
  addElement("Zicronium",     "Zr",  40, 5,  4, 1.33, 206);
  addElement("Niobium",       "Nb",  41, 5,  5, 1.60, 198);
  addElement("Molybdenum",    "Mo",  42, 5,  6, 2.16, 190);
  addElement("Technetium",    "Tc",  43, 5,  7, 1.90, 183);
  addElement("Ruthenium",     "Ru",  44, 5,  8, 2.20, 178);
  addElement("Rhodium",       "Rh",  45, 5,  9, 2.28, 173);
  addElement("Palladium",     "Pd",  46, 5, 10, 2.20, 169);
  addElement("Silver",        "Ag",  47, 5, 11, 1.93, 165);
  addElement("Cadmium",       "Cd",  48, 5, 12, 1.69, 161);
  addElement("Indium",        "In",  49, 5, 13, 1.78, 156);
  addElement("Tin",           "Sn",  50, 5, 14, 1.96, 145);
  addElement("Antimony",      "Sb",  51, 5, 15, 2.05, 133);
  addElement("Tellurium",     "Te",  52, 5, 16, 2.10, 123);
  addElement("Iodine",        "I",   53, 5, 17, 2.66, 115);
  addElement("Xenon",         "Xe",  54, 5, 18, 2.60, 108);
  addElement("Caesium",       "Cs",  55, 6,  1, 0.79, 298);
  addElement("Barium",        "Ba",  56, 6,  2, 0.89, 253);
  addElement("Lanthanum",     "La",  57, 6, -1, 1.10,  -1);
  addElement("Cerium",        "Ce",  58, 6, -1, 1.12,  -1);
  addElement("Praseodymium",  "Pr",  59, 6, -1, 1.13, 247);
  addElement("Neodymium",     "Nd",  60, 6, -1, 1.14, 206);
  addElement("Promethium",    "Pm",  61, 6, -1,   -1, 205);
  addElement("Samarium",      "Sm",  62, 6, -1, 1.17, 238);
  addElement("Europium",      "Eu",  63, 6, -1,   -1, 231);
  addElement("Gadolinium",    "Gd",  64, 6, -1, 1.20, 233);
  addElement("Terbium",       "Tb",  65, 6, -1,   -1, 223);
  addElement("Dysprosium",    "Dy",  66, 6, -1, 1.22, 228);
  addElement("Holmium",       "Ho",  67, 6, -1, 1.23, 226);
  addElement("Erbium",        "Er",  68, 6, -1, 1.24, 226);
  addElement("Thulium",       "Tm",  69, 6, -1, 1.25, 222);
  addElement("Ytterbium",     "Yb",  70, 6, -1,   -1, 222);
  addElement("Lutetium",      "Lu",  71, 6, -1, 1.27, 217);
  addElement("Hafnium",       "Hf",  72, 6,  4, 1.30, 208);
  addElement("Tantalum",      "Ta",  73, 6,  5, 1.50, 200);
  addElement("Tungsten",      "W",   74, 6,  6, 2.36, 193);
  addElement("Rhenium",       "Re",  75, 6,  7, 1.90, 188);
  addElement("Osmium",        "Os",  76, 6,  8, 2.20, 185);
  addElement("Iridium",       "Ir",  77, 6,  9, 2.20, 180);
  addElement("Platinum",      "Pt",  78, 6, 10, 2.28, 177);
  addElement("Gold",          "Au",  79, 6, 11, 2.54, 174);
  addElement("Mercury",       "Hg",  80, 6, 12, 2.00, 171);
  addElement("Thallium",      "Tl",  81, 6, 13, 1.62, 156);
  addElement("Lead",          "Pb",  82, 6, 14, 2.33, 154);
  addElement("Bismuth",       "Bi",  83, 6, 15, 2.02, 143);
  addElement("Polonium",      "Po",  84, 6, 16, 2.00, 135);
  addElement("Astatine",      "At",  85, 6, 17, 2.20, 127);
  addElement("Radon",         "Rn",  86, 6, 18,   -1, 120);
  addElement("Francium",      "Fr",  87, 7,  1, 0.70,  -1);
  addElement("Radium",        "Fa",  88, 7,  2, 0.90,  -1);
  addElement("Actinium",      "Ac",  89, 7, -2, 1.10,  -1);
  addElement("Thorium",       "Th",  90, 7, -2, 1.30,  -1);
  addElement("Protactinum",   "Pa",  91, 7, -2, 1.50,  -1);
  addElement("Uranium",       "U",   92, 7, -2, 1.38,  -1);
  addElement("Neptunium",     "Np",  93, 7, -2, 1.36,  -1);
  addElement("Plutonium",     "Pu",  94, 7, -2, 1.28,  -1);
  addElement("Americium",     "Am",  95, 7, -2, 1.30,  -1);
  addElement("Curium",        "Cm",  96, 7, -2, 1.30,  -1);
  addElement("Berkelium",     "Bk",  97, 7, -2, 1.30,  -1);
  addElement("Californium",   "Cf",  98, 7, -2, 1.30,  -1);
  addElement("Einsteinium",   "Es",  99, 7, -2, 1.30,  -1);
  addElement("Fermium",       "Fm", 100, 7, -2, 1.30,  -1);
  addElement("Mendelevium",   "Md", 101, 7, -2, 1.30,  -1);
  addElement("Nobelium",      "No", 102, 7, -2, 1.30,  -1);
  addElement("Lawrencium",    "Lr", 103, 7, -2,   -1,  -1);
  addElement("Rutherfordium", "Rf", 104, 7,  4,   -1,  -1);
  addElement("Dubnium",       "Db", 105, 7,  5,   -1,  -1);
  addElement("Seaborgium",    "Sg", 106, 7,  6,   -1,  -1);
  addElement("Bohrium",       "Bh", 107, 7,  7,   -1,  -1);
  addElement("Hassium",       "Hs", 108, 7,  8,   -1,  -1);
  addElement("Meitnerium",    "Mt", 109, 7,  9,   -1,  -1);
  addElement("Darmstadtium",  "Ds", 110, 7, 10,   -1,  -1);
  addElement("Roentgenium",   "Rg", 111, 7, 11,   -1,  -1);
  addElement("Copernicium",   "Cn", 112, 7, 12,   -1,  -1);
  addElement("Nihonium",      "Nh", 113, 7, 13,   -1,  -1);
  addElement("Flerovium",     "Fl", 114, 7, 14,   -1,  -1);
  addElement("Moscovium",     "Mc", 115, 7, 15,   -1,  -1);
  addElement("Livermonium",   "Lv", 116, 7, 16,   -1,  -1);
  addElement("Tennessine",    "Ts", 117, 7, 17,   -1,  -1);
  addElement("Oganesson",     "Og", 118, 7, 18,   -1,  -1);
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
