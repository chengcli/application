#ifndef SRC_MONITOR_H
#define SRC_MONITOR_H

#include <iostream>
#include <string>
#include <cstring>

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
    virtual void Log(std::string_view msg, uint8_t code) const;

    //! Write an error message to the error device
    /*!
     * End-of-line character is not appended to the message.
     *
     * @param msg      String message to be written to cout
     */
    virtual void Error(std::string_view msg, uint8_t code) const;

    //! Write a warning message to the log device
    /*!
     * End-of-line character is appended to the message.
     * 
     * @param msg     String message to be written to cout
     */
    virtual void Warn(std::string_view msg, uint8_t code) const;

    void Enter() {
        sections_.push_back(0);
        Log("begin",0);
    }

    void Leave() {
        sections_.pop_back();
        Log("end",0);
    }

    bool SetLogFile(std::string_view fname) {
        log_device_ = make_unique<std::ofstream>(fname, std::ios::out);
    }

    bool SetErrFile(std::string_view fname) {
        err_device_ = make_unique<std::ofstream>(fname, std::ios::out);
    }

protected:
    virtual void getTimeStamp() const;

    virtual std::string getSectionID() const;

    void advance();

    std::unique_pointer<std::ostream>  log_device_;
    std::unique_pointer<std::ostream>  err_device_;

    std::string module_name_;

    std::vector<uint32_t> sections_;
};

using MonitorPtr = std::unique_ptr<Monitor>;

using MonitorMap = std::map<std::string, MonitorPtr>;

#endif  // SRC_MONITOR_H
