#ifndef SRC_MESSAGE_HPP_
#define SRC_MESSAGE_HPP_

#include <memory>
#include <thread>
#include <string>

//! Class to carry out messages
class Message
{
public:
    Message();

    Message(const Message& r) = delete;
    Message& operator=(const Message& r) = delete;

    //! Set an error condition in the application class without
    //! throwing an exception.
    /*!
     * This routine adds an error message to the end of the stack of errors
     * @param r    Procedure name which is generating the error condition
     * @param msg  Descriptive message of the error condition.
     *
     * If only one argument is specified, that string is used as the
     * entire message.
     * @ingroup errorhandling
     */
    void AddError(std::string_view r, const std::string& msg="");

    //! Return the number of errors that have been encountered so far.
    /*!
     * @ingroup errorhandling
     */
    int CountErrors() const;

    //! Discard the last error message
    /*!
     * This routine eliminates the last exception to be
     * added to that stack.
     *
     * @ingroup errorhandling
     */
    void PopError();

    //! Retrieve the last error message in a string
    /*!
     * This routine will retrieve the last error message and return it in
     * the return string.
     *
     * @ingroup errorhandling
     */
    std::string GetLastErrorMessage();

    //!  Prints all of the error messages using writelog
    /*!
     * Print all of the error messages using function Writelog.
     * This routine writes out all of the error messages and then
     * clears them from internal storage.
     *
     * @ingroup errorhandling
     */
    void DumpErrors();

    //! @copy LogErrors
    void DumpErrors(std::ostream& f);

    //!  Write a message to the screen.
    /*!
     * The string may be of any length, and may contain end-of-line
     * characters. The advantage of
     * using writelog over writing directly to the standard output is that
     * messages written with writelog will display correctly even when
     * other application that do not have a standard output stream.
     *
     * @param msg  c++ string to be written to the screen
     * @ingroup textlogs
     */
    void Write(std::string_view msg);

    //!  Write a warning message to the screen.
    /*!
     * @param warning  String specifying type of warning; see Logger::warn()
     * @param msg  String to be written to the screen
     * @ingroup textlogs
     */
    void Warn(std::string_view warning, std::string_view msg);

    //! Install a logger.
    /*!
     * Called by the language interfaces to install an appropriate logger.
     * The logger is used for the writelog() function
     *
     * @param log_writer pointer to a logger object
     */
    void SetLogger(Logger* log_writer);

protected:
    //! write an endline character
    void write_endl();

    //! Current list of error messages
    std::vector<std::string> error_messages_;

    //! Current pointer to the log_writer
    std::unique_ptr<Logger> log_writer_;
};

using MessagePtr = std::unique_ptr<Message>;
using ThreadMessageMap = std::map<std::thread::id, MessagePtr>

//! Class that stores thread messages for each thread, and retrieves them
//! based on the thread id.
class ThreadMessage
{
public:
    //! Constructor
    ThreadMessage() {}

    //! Provide a pointer dereferencing overloaded operator
    /*!
     * @returns a pointer to Message
     */
    Message* operator->();

    //! Remove a local thread message
    void RemoveMyMessage();

private:
    //! Thread Msg Map
    ThreadMessageMap msg_map_;
};


#endif  // SRC_MESSAGE_HPP_
