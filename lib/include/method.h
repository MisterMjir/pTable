#ifndef METHOD
#define METHOD

#include <string>
#include <vector>

#define METHOD_ARGS const std::vector<std::string> &

struct Method
{
  std::string name;
  void (*method)(METHOD_ARGS);
  unsigned int argc;
  std::string info;
  void (*help)();
};

#endif
