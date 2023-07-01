// C/C++
// first 2x macros and signal() are the only ISO C features; rest are POSIX C extensions
#include <csignal>    // SIGTERM, SIGINT, SIGALARM, signal(), sigemptyset(), ...
#include <iostream>
#include <unistd.h>   // alarm() Unix OS utility; not in C standard --> no <cunistd>
#include <memory>

// application
#include "globals.hpp"
#include "signal_handler.hpp"

#ifdef MPI_PARALLEL
#include <mpi.h>
#endif

static std::mutex sig_mutex;

SignalHandler* SignalHandler::GetInstance() {
  // RAII
  std::unique_lock<std::mutex> lock(sig_mutex);

  if (SignalHandler::mysig_ == nullptr) {
    SignalHandler::mysig_ = new SignalHandler();
  }

  return mysig_;
}

void SignalHandler::Destroy() {
  std::unique_lock<std::mutex> lock(sig_mutex);

  if (SignalHandler::mysig_ != nullptr) {
    delete SignalHandler::mysig_;
    SignalHandler::mysig_ = nullptr;
  }
}

SignalHandler::SignalHandler() {
  for (int n=0; n<NSIGNAL; n++) {
    signalflag_[n]=0;
  }
  // C++11 standard guarantees that <csignal> places C-standard signal.h contents in std::
  // namespace. POSIX C extensions are likely only placed in global namespace (not std::)
  std::signal(SIGTERM, SetSignalFlag);
  std::signal(SIGINT,  SetSignalFlag);
  std::signal(SIGALRM, SetSignalFlag);

  // populate set of signals to block while the handler is running; prevent premption
  sigemptyset(&mask_);
  sigaddset(&mask_, SIGTERM);
  sigaddset(&mask_, SIGINT);
  sigaddset(&mask_, SIGALRM);
}

int SignalHandler::CheckSignalFlags() {
  // Currently, only checking for nonzero return code at the end of each timestep in
  // main.cpp; i.e. if an issue prevents a process from reaching the end of a cycle, the
  // signals will never be handled by that process / the solver may hang
  int ret = 0;
  sigprocmask(SIG_BLOCK, &mask_, nullptr);
#ifdef MPI_PARALLEL
  MPI_Allreduce(MPI_IN_PLACE,
                const_cast<void *>(reinterpret_cast<volatile void *>(signalflag_)),
                NSIGNAL, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
#endif
  for (int n=0; n<NSIGNAL; n++)
    ret += signalflag_[n];
  sigprocmask(SIG_UNBLOCK, &mask_, nullptr);
  return ret;
}

int SignalHandler::GetSignalFlag(int s) {
  int ret=-1;
  switch(s) {
    case SIGTERM:
      ret=signalflag_[ITERM];
      break;
    case SIGINT:
      ret=signalflag_[IINT];
      break;
    case SIGALRM:
      ret=signalflag_[IALRM];
      break;
    default:
      // nothing
      break;
  }
  return ret;
}

void SignalHandler::SetSignalFlag(int s) {
  // Signal handler functions must have C linkage; C++ linkage is implemantation-defined
  switch(s) {
    case SIGTERM:
      signalflag_[ITERM]=1;
      std::signal(s, SetSignalFlag);
      break;
    case SIGINT:
      signalflag_[IINT]=1;
      std::signal(s, SetSignalFlag);
      break;
    case SIGALRM:
      signalflag_[IALRM]=1;
      std::signal(s, SetSignalFlag);
      break;
    default:
      // nothing
      break;
  }
  return;
}

void SignalHandler::SetWallTimeAlarm(int t) {
  alarm(t);
  return;
}

void SignalHandler::CancelWallTimeAlarm() {
  alarm(0);
  return;
}

int SignalHandler::signalflag_[SignalHandler::NSIGNAL];
SignalHandler* SignalHandler::mysig_ = nullptr;
