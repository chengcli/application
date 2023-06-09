// application 
#include <application/application.hpp>

// define global variables
namespace Globals {
  int my_rank, nranks;
}

void func2() {
  Application::Logger log("B");
  auto app = Application::GetMonitor(log);

  app->Log("First step");
  app->Log("Second step");
}

void func1() {
  Application::Logger log("A");
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
  app->InitMonitorLog("main", "main.out");
  app->InitMonitorErr("main", "main.err");

  app->InitMonitorLog("A", "main.out");
  app->InitMonitorErr("A", "main.err");

  app->InitMonitorLog("B", "main.out");
  app->InitMonitorErr("B", "main.err");

  std::cout << app->CountMonitors() << std::endl;

  func1();

  Application::Destroy();
}
