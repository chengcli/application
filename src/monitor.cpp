// C/C++
#include <fstream>
#include <iostream>
#include <memory>

// application
#include "application.hpp"
#include "monitor.hpp"

static std::mutex section_mutex;

void Monitor::Log(std::string const& msg, int code) {
  advance();
  char buf[80];
  snprintf(buf, sizeof(buf), "Log, %s, %s, %s, \"%s\", %d\n",
           getTimeStamp().c_str(), name_.c_str(), getSectionID().c_str(),
           msg.c_str(), code);
  (*log_device_) << buf;
}

void Monitor::Error(std::string const& msg, int code) {
  advance();
  char buf[80];
  snprintf(buf, sizeof(buf), "Error; %s, %s, %s, \"%s\", %d\n",
           getTimeStamp().c_str(), name_.c_str(), getSectionID().c_str(),
           msg.c_str(), code);
  (*err_device_) << buf;
}

void Monitor::Warn(std::string const& msg, int code) {
  advance();
  char buf[80];
  snprintf(buf, sizeof(buf), "Warn, %s, %s, %s, \"%s\", %d\n",
           getTimeStamp().c_str(), name_.c_str(), getSectionID().c_str(),
           msg.c_str(), code);
  (*log_device_) << buf;
}

void Monitor::Enter() {
  std::unique_lock<std::mutex> lock(section_mutex);

  sections_.push_back(0);
}

void Monitor::Leave() {
  std::unique_lock<std::mutex> lock(section_mutex);

  sections_.pop_back();
}

bool Monitor::SetLogOutput(std::string const& fname) {
  auto app = Application::GetInstance();

  if (app->HasDevice(fname)) {
    log_device_ = app->GetDevice(fname);
  } else {
    log_device_ = std::make_shared<std::ofstream>(fname, std::ios::out);
    app->InstallDevice(fname, log_device_);
  }

  return true;
}

bool Monitor::SetErrOutput(std::string const& fname) {
  auto app = Application::GetInstance();

  if (app->HasDevice(fname)) {
    err_device_ = app->GetDevice(fname);
  } else {
    err_device_ = std::make_shared<std::ofstream>(fname, std::ios::out);
    app->InstallDevice(fname, err_device_);
  }

  return true;
}

std::string Monitor::getTimeStamp() const { return "XXX"; }

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

std::vector<uint32_t> Monitor::sections_ = {};
