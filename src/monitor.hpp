#ifndef SRC_MONITOR_H
#define SRC_MONITOR_H

#include <iostream>
#include <string>

class Monitor 
{
protected:
    //! Protected ctor access thru static member function Instance
    
public:
    explicit Monitor(std::string name):
        module_name_(name)
    {}

    //! Destructor - empty
    virtual ~Monitor() {}

    //! Write a log message to the log device
    /*!
     * End-of-line character is appended to the message.
     *
     * @param msg      String message to be written to cout
     */
    virtual void Log(std::string_view msg, uint8_t code = 0) const {
        advanceSection();
        (*log_device_) << fmt::format("Log; {}; {}.{}; {}; {}\n", 
            getTimeStamp(), module_name_, getSectionID(), msg, code);
    }

    //! Write an error message to the error device
    /*!
     * End-of-line character is not appended to the message.
     *
     * @param msg      String message to be written to cout
     */
    virtual void Error(std::string_view msg, uint8_t code = 0) const {
        advanceSection();
        (*err_device_) << fmt::format("Error; {}; {}.{}; {}; {}",
            getTimeStamp(), module_name_, getSectionID(), msg, code);
    }

    //! Write a warning message to the log device
    /*!
     * End-of-line character is appended to the message.
     * 
     * @param msg     String message to be written to cout
     */
    virtual void Warn(std::string_view msg, uint8_t code = 0) const {
        advanceSection();
        (*log_device_) << fmt::format("Warning: {}; {}.{}; {}; {}\n",
            getTimeStamp(), module_name_, getSectionID(), msg, code);
    }

    void Enter() {
        sections_.push_back(1);
    }

    void Leave() {
        sections_.pop_back();
    }

    void SetLogFile(std::string_view fname) {
        log_device_ = make_unique<std::ofstream>(fname, std::ios::out);
    }

    void SetErrFile(std::string_view fname) {
        err_device_ = make_unique<std::ofstream>(fname, std::ios::out);
    }

protected:
    virtual void getTimeStamp() const {
      return "XXX";
    }

    virtual std::string getSectionID() const {
      if (sections_.size() == 0) {
        return "0.";
      } else {
        std::string str;
        for (auto i : sections_) {
          str += std::to_string(i) + '.'
        }
        return str;
      }
    }

    void advanceSection() {
        sections_.back() += 1;
    }

    std::unique_pointer<std::ostream>  log_device_;
    std::unique_pointer<std::ostream>  err_device_;

    std::string module_name_;

    uint32_t  cur_depth_;
    uint32_t  max_depth_;
    std::vector<uint32_t> sections_;
};

using MonitorPtr = std::unique_ptr<Monitor>;

using ModuleMonitorMap = std::map<std::thread::id, MonitorPtr>

class ModuleMonitor
{
public:
    //! Constructor
    ModuleMoniotr() {}

    //! Provide a pointer dereferencing overloaded operator
    /*!
     * @returns a pointer to Message
     */
    Monitor* operator->();

private:
};

class Logger {
public:
    Logger(std::string name) {
        auto iter = all_monitors_.find(name);
        if (iter != all_monitors_.end()) {
          iter->second->Enter();
          cur_monitor_ = iter->second;
        } else {
          cur_monitor_ = nullptr;
        }
    }

    ~Logger() {
        if (cur_monitor_ != nullptr)
          cur_monitor_->Leave();
    }

protected:
    std::weak_pointer<Monitor> cur_monitor_
};

void increment_id(std::string &str);

#endif  // SRC_MONITOR_H
