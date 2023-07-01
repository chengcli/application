#ifndef SRC_SIGNAL_HANDLER_HPP_
#define SRC_SIGNAL_HANDLER_HPP_

// C/C++
#include <csignal>   // sigset_t POSIX C extension
#include <cstdint>   // std::int64_t

class SignalHandler {
protected:
  SignalHandler();

public:
  enum {
    ITERM = 0,
    IINT = 1,
    IALRM = 2,
    NSIGNAL = 3,
  };

  static SignalHandler* GetInstance();
  static void Destroy();
  static void SetSignalFlag(int s);

  int CheckSignalFlags();
  int GetSignalFlag(int s);
  void SetWallTimeAlarm(int t);
  void CancelWallTimeAlarm();

protected:
  static int signalflag_[NSIGNAL];
  sigset_t mask_;

private:
  //! Pointer to the single SingnalHandler instance
  static SignalHandler* mysig_;
};

#endif // SRC_SIGNAL_HANDLER_HPP_
