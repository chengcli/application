#ifndef SRC_MONITOR_HPP_
#define SRC_MONITOR_HPP_

// C/C++
#include <cstring>
#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Monitor {
 protected:
  //! Protected ctor access thru static member function Instance

 public:
  explicit Monitor(std::string name) : name_(name) {}

  //! Destructor - empty
  virtual ~Monitor() {}

  //! Write a log message to the log device
  /*!
   * End-of-line character is appended to the message.
   *
   * @param msg      String message to be written to cout
   */
  virtual void Log(std::string const& msg, int code = 0);

  //! Write an error message to the error device
  /*!
   * End-of-line character is not appended to the message.
   *
   * @param msg      String message to be written to cout
   */
  virtual void Error(std::string const& msg, int code = 0);

  //! Write a warning message to the log device
  /*!
   * End-of-line character is appended to the message.
   *
   * @param msg     String message to be written to cout
   */
  virtual void Warn(std::string const& msg, int code = 0);

  void Enter();

  void Leave();

  bool SetLogOutput(std::string const& fname);

  bool SetErrOutput(std::string const& fname);

 protected:
  virtual std::string getTimeStamp() const;

  virtual std::string getSectionID() const;

  void advance();

  std::shared_ptr<std::ostream> log_device_;
  std::shared_ptr<std::ostream> err_device_;

  std::string name_;

  static std::vector<uint32_t> sections_;
};

using MonitorPtr = std::unique_ptr<Monitor>;

using MonitorMap = std::map<std::string, MonitorPtr>;

using DevicePtr = std::shared_ptr<std::ostream>;

using DeviceMap = std::map<std::string, DevicePtr>;

#endif  // SRC_MONITOR_HPP_
