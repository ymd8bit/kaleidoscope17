#include "kaleidoscope17/driver.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace kaleidoscope17;

int main(int argc, char const *argv[])
{
  // parse the given args in a vector without the executable name;
  const std::vector<std::string> args(argv + 1, argv + argc);

  if (args.size() >= 4) {
    EXCEPTION("Number of the given input arguments should be 0 - 3");
  } else if (args.size() == 0) {
    /// run compiler with standard out.
    Driver driver(std::cin, std::cout, true); // debug_mode true.
    driver.mainloop();
  } else {
    /// run compiler with the given text file by the 1st argument.
    const std::string file_name = args[0];
    std::ifstream ifstrm(file_name);

    if (!ifstrm.is_open()) {
      EXCEPTION("failed to open the test input text...");
    }

    /// debug mode flag
    bool debug_mode = false;
    bool pass_enable = false;

    if (args.size() == 2) {
      if (args[1] == "-debug") {
        debug_mode = true;
      } else if (args[1] == "-pass") {
        pass_enable = true;
      } else {
        EXCEPTION("failed to parse unknow mode: " + args[1]);
      }
    }

    if (args.size() == 3) {
      if ((args[1] == "-debug" && args[2] == "-pass") ||
          (args[1] == "-pass" && args[2] == "-debug")) {
        debug_mode = true;
        pass_enable = true;
      } else {
        EXCEPTION("failed to parse unknow modes: " + args[1] + " and " +
                  args[2]);
      }
    }

    Driver driver(ifstrm, std::cout, debug_mode);
    driver.mainloop();
  }

  return 0;
}
