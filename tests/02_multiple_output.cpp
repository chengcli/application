// application 
#include <application/application.hpp>

void func2() {
  Application::Logger app("B");

  app->Log("First step");
  app->Log("Second step");
}

void func1() {
  Application::Logger app("A");

  app->Log("First step");
  app->Log("Second step");

  func2();
}

int main(int argc, char **argv) {
  auto app = Application::GetInstance();

  app->InstallMonitor("main", "main.out", "main.err");
  app->InstallMonitor("A", "A.out", "A.err");
  app->InstallMonitor("B", "B.out", "B.err");

  std::cout << app->CountMonitors() << std::endl;

  func1();

  Application::Destroy();
}