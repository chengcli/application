// debugger headers
#include <debugger.hpp>

// define global variables
namespace Globals {
  int my_rank, nranks;
}

void func2() {
  Application::Logger log("main");
  auto app = Application::GetMonitor(log);

  app->Log("First step");
  app->Log("Second step");
}

void func1() {
  Application::Logger log("main");
  auto app = Application::GetMonitor(log);

  app->Log("First step");
  app->Log("Second step");

  func2();
}

int main(int argc, char **argv) {
  // no MPI
  Globals::my_rank = 0;
  Globals::nranks = 0;

  auto app = Application::GetInstance();
  app->InitMonitorLog("main", "main.out")
  app->InitMonitorErr("main", "main.err")

  func1();
}
