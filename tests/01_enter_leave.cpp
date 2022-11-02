// debugger headers
#include <debugger.hpp>

// define global variables
namespace Globals {
  int my_rank, nranks;
}

int main(int argc, char **argv) {
  // no MPI
  Globals::my_rank = 0;
  Globals::nranks = 0;

  pdebug = std::make_unique<Debugger>(2);

  pdebug->Enter("main");
    pdebug->Enter("level1");
      pdebug->Enter("level2");
      pdebug->Leave();
    pdebug->Leave();
  pdebug->Leave();
}
