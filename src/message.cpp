// C/C++
#include <fstream>
#include <sstream>
#include <mutex>

#include "logger.hpp"
#include "message.h"

//! Mutex for input directory access
static std::mutex dir_mutex;

Message::Message()
{
    // install a default Monitor that writes log/error messages
    // output / standard error
    monitor_ = make_unique<Monitor>();
}

void Message::AddError(const std::string& r, const std::string& msg)
{
    if (msg.size() != 0) {
        error_messages_.push_back(
            "************************************************\n\n"
            "Error at Procedure: " + r +
            "\nError:     " + msg + "\n");
    } else {
        error_messages_.push_back(
            "************************************************\n\n"
            "Error at Procedure: " + r + "\n");
    }
}

int Message::CountErrors() const
{
    return static_cast<int>(error_messages_.size());
}

void Messages::PopError()
{
    if (!error_messages_.empty()) {
        error_messages_.pop_back();
    }
}

std::string Messages::GetLastErrorMessage()
{
    if (!error_messages_.empty()) {
        return error_messages_.back();
    } else  {
        return "<No Error>";
    }
}

void Messages::DumpErrors()
{
    for (size_t j = 0; j < error_messages.size(); j++) {
        Write(error_message[j]);
        write_endl();
    }
    error_messages.clear();
}

void Messages::DumpErrors(std::ostream& f)
{
    for (size_t j = 0; j < errorMessage.size(); j++) {
        f << error_messages[j] << std::endl;
    }
    error_messages.clear();
}

void Message::Write(std::string_view msg)
{
    monitor_->Write(msg);
}

void Message::Warn(std::string_view warning, std::string_view msg)
{
    monitor_->Warn(warning, msg);
}

void Message::SetMonitor(Monitor* monitor)
{
    monitor_.reset(monitor);
}

//! Mutex for access to string messages
static std::mutex msg_mutex;

Application::Message* ThreadMessage::operator ->()
{
    // RAII
    std::unique_lock<std::mutex> lock(msg_mutex);

    std::thread::id cur_id = std::this_thread::get_id();
    auto iter = msg_map_.find(cur_id);
    if (iter != msg_map_.end()) {
        return iter->second.get();
    }

    MessagePtr msg(new Message());
    msg_map_.insert({cur_id, msg});

    return msg.get();
}

void ThreadMessage::RemoveMyMessage()
{
    // RAII
    std::unique_lock<std::mutex> lock(msg_mutex);

    std::thread::id cur_id = std::this_thread::get_id();
    auto iter = msg_map_.find(cur_id);
    if (iter != msg_map_.end()) {
        msg_map_.erase(iter);
    }
}
