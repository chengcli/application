#ifndef SRC_MONITOR_H
#define SRC_MONITOR_H

// C/C++
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <map>
#include <vector>

class Monitor 
{
protected:
    //! Protected ctor access thru static member function Instance
    
public:
    explicit Monitor(std::string name):
        name_(name)
    {}

    //! Destructor - empty
    virtual ~Monitor() {}

    //! Write a log message to the log device
    /*!
     * End-of-line character is appended to the message.
     *
     * @param msg      String message to be written to cout
     */
    virtual void Log(std::string const& msg, int code);

    //! Write an error message to the error device
    /*!
     * End-of-line character is not appended to the message.
     *
     * @param msg      String message to be written to cout
     */
    virtual void Error(std::string const& msg, int code);

    //! Write a warning message to the log device
    /*!
     * End-of-line character is appended to the message.
     * 
     * @param msg     String message to be written to cout
     */
    virtual void Warn(std::string const& msg, int code);

    void Enter() {
        sections_.push_back(0);
        Log("begin",0);
    }

    void Leave() {
        sections_.pop_back();
        Log("end",0);
    }

    bool SetLogFile(std::string_view fname) {
        log_device_ = std::make_unique<std::ofstream>(
            fname, std::ios::out);

        return true;
    }

    bool SetErrFile(std::string_view fname) {
        err_device_ = std::make_unique<std::ofstream>(
            fname, std::ios::out);

        return true;
    }

protected:
    virtual std::string getTimeStamp() const;

    virtual std::string getSectionID() const;

    void advance();

    std::unique_ptr<std::ostream>  log_device_;
    std::unique_ptr<std::ostream>  err_device_;

    std::string name_;

    std::vector<uint32_t> sections_;
};

using MonitorPtr = std::unique_ptr<Monitor>;

using MonitorMap = std::map<std::string, MonitorPtr>;

#endif  // SRC_MONITOR_H
