#ifndef PTABLE_METHOD
#define PTABLE_METHOD

#include <string>
#include <vector>

#define PTABLE_METHOD_ARGS const std::vector<std::string> &

struct Method
{
  std::string name;
  void (*method)(PTABLE_METHOD_ARGS);
  unsigned int argc;
  std::string info;
  void (*help)();
};

#endif
