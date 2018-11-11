#include "kaleidoscope17/driver.h"

#include <fstream>
#include <iostream>
using namespace kaleidoscope17;

constexpr const bool DEBUG = true;

int main(int argc, char const *argv[])
{
  if (DEBUG)
  {
    std::ifstream ifstrm("test.txt");

    if (!ifstrm.is_open())
    {
      std::cout << "failed to open test input text..." << std::endl;
      return 1;
    }

    Driver driver(ifstrm);
    driver.mainloop();
  }
  else
  {

    Driver driver(std::cin);
    driver.mainloop();
  }

  return 0;
}
