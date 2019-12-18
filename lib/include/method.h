#ifndef METHOD
#define METHOD

#include <string>
#include <vector>

struct Method
{
  std::string name;
  void (*method)(const std::vector<std::string> &);
  unsigned int argc;
  std::string info;
  void (*help)();
};

#endif
