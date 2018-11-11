#include "kaleidoscope17/driver.h"

#include <fstream>
#include <iostream>
#include <string>
using namespace kaleidoscope17;

int main(int argc, char const *argv[])
{
  if (argc >= 3) {
    EXCEPTION("Number of the given input arguments should be 0 or 1...");
  } else if (argc == 1) {
    /// run compiler with standard out.
    Driver driver(std::cin);
    driver.mainloop();
  } else {
    /// run compiler with text file.
    const std::string file_name = argv[1];
    std::ifstream ifstrm(file_name);

    if (!ifstrm.is_open()) {
      EXCEPTION("failed to open the test input text...");
    }

    Driver driver(ifstrm);
    driver.mainloop();
  }

  return 0;
}
