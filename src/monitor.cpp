// application
#include "monitor.hpp"

void Monitor::Log(std::string const& msg, int code) {
    advance();
    char buf[80];
    snprintf(buf, sizeof(buf), "Log; %s; %s.%s; %s; %d\n", 
        getTimeStamp().c_str(),
        name_.c_str(),
        getSectionID().c_str(),
        msg.c_str(),
        code);
    (*log_device_) << buf;
}

void Monitor::Error(std::string const& msg, int code) {
    advance();
    char buf[80];
    snprintf(buf, sizeof(buf), "Error; %s; %s.%s; %s; %d\n", 
        getTimeStamp().c_str(),
        name_.c_str(),
        getSectionID().c_str(),
        msg.c_str(),
        code);
    (*err_device_) << buf;
}

void Monitor::Warn(std::string const& msg, int code) {
    advance();
    char buf[80];
    snprintf(buf, sizeof(buf), "Error; %s; %s.%s; %s; %d\n", 
        getTimeStamp().c_str(),
        name_.c_str(),
        getSectionID().c_str(),
        msg.c_str(),
        code);
    (*log_device_) << buf;
}

std::string Monitor::getTimeStamp() const {
  return "XXX";
}

std::string Monitor::getSectionID() const {
  if (sections_.size() == 0) {
    return "0.";
  } else {
    std::string str;
    for (auto i : sections_) {
      str += std::to_string(i) + '.';
    }
    return str;
  }
}

void Monitor::advance() {
    if (sections_.size() != 0) {
      sections_.back() += 1;
    } else {
      sections_.push_back(1);
    }
}
