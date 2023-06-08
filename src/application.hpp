#ifndef SRC_APPLICATION_HPP_
#define SRC_APPLICATION_HPP_

#include "cantera/base/config.h"
#include "cantera/base/logger.h"

class Application
{
protected:
    //! Constructor for class sets up the initial conditions
    //! Protected ctor access thru static member function Instance
    Application();

public:
    class Logger {
      public:
        Logger(std::string name);
        ~Logger();

        Monitor* GetMonitor() {
          return cur_monitor_.get();
        }

      protected:
        std::weak_pointer<Monitor> cur_monitor_;
    };

    static std::string FatalError(std::string_view fmt) {
        char buf[80];
        return 
    }

    //! Return a pointer to the one and only instance of class Application
    /*!
     * If the Application object has not yet been created, it is created
     */
    static Application* GetInstance();

    static Monitor* GetMonitor(Logger const& log) {
        return log.GetMonitor();
    }

    //! Destructor for class deletes global data
    virtual ~Application() {}

    //! Static function that destroys the application class's data
    static void Destroy();

    bool InitMonitorLog(std::string_view mod, std::string_view fname);

    bool SetMonitorErrFile(std::string_view mod, std::string_view fname);

    //!  Add a directory to the data file search path.
    /*!
     * @ingroup inputfiles
     *
     * @param dir  String name for the directory to be added to the search path
     */
    void AddDataDirectory(const std::string& dir);

    //! Find an input file.
    /*!
     * This routine will search for a file in the default locations specified
     * for the application. See the routine setDefaultDirectories() listed
     * above. The first directory searched is usually the current working
     * directory.
     *
     * The default set of directories will not be searched if an absolute path
     * (for example, one starting with `/` or `C:\`) or a path relative to the
     * user's home directory (for example, starting with `~/`) is specified.
     *
     * The presence of the file is determined by whether the file can be
     * opened for reading by the current user.
     *
     * @param name Name of the input file to be searched for
     * @return  The absolute path name of the first matching file
     *
     * If the file is not found a CanteraError exception is thrown.
     *
     * @ingroup inputfiles
     */
    std::string FindInputFile(const std::string& name);

    //! Get the Cantera data directories
    /*!
     * This routine returns a string including the names of all the
     * directories searched by Cantera for data files.
     *
     * @param sep Separator to use between directories in the string
     * @return A string of directories separated by the input sep
     *
     * @ingroup inputfiles
     */
    std::string GetDataDirectories(const std::string& sep) {
      std::stringstream ss;
      for (size_t i = 0; i < inputDirs.size(); ++i) {
        if (i != 0) {
          ss << sep;
        }
        ss << inputDirs[i];
      }
      return ss.str();
    }

    //! Set the versions of Python to try when loading user-defined extensions,
    //! in order of preference. Separate multiple versions with commas, for example
    //! `"3.11,3.10"`.
    //! @since New in Cantera 3.0
    void SearchPythonVersions(const string& versions);

    void WriteLog(const std::string& msg) {
        pMessenger->WriteLog(msg);
    }

    void WarnLog(const std::string& warning, const std::string& msg) {
        pMessenger->WarnLog(warning, msg);
    }

    //! Print a warning indicating that *method* is deprecated. Additional
    //! information (removal version, alternatives) can be specified in
    //! *extra*. Deprecation warnings are printed once per method per
    //! invocation of the application.
    void WarnDeprecated(std::string_view method, const std::string& extra="");

    //! Globally disable printing of deprecation warnings. Used primarily to
    //! prevent certain tests from failing.
    void SuppressDeprecationWarnings() {
        suppress_deprecation_warnings_ = true;
        fatal_deprecation_warnings_ = false;
    }

    //! Turns deprecation warnings into exceptions. Activated within the test
    //! suite to make sure that no deprecated methods are being used.
    void MakeDeprecationWarningsFatal() {
        fatal_deprecation_warnings_ = true;
    }

    //! Generate a general purpose warning; repeated warnings are not suppressed
    //! @param warning  Warning type; see Logger::warn()
    //! @param method  Name of method triggering the warning
    //! @param extra  Additional information printed for the warning
    void Warn(std::string_view warning,
              std::string_view method, const std::string& extra="");

    //! Globally disable printing of (user) warnings. Used primarily to
    //! prevent certain tests from failing.
    void SuppressWarnings() {
        suppress_warnings_ = true;
        fatal_warnings_ = false;
    }

    //! Returns `true` if warnings should be suppressed.
    bool IsWarningsSuppressed() {
        return suppress_warnings_;
    }

    //! Turns Cantera warnings into exceptions. Activated within the test
    //! suite to make sure that your warning message are being raised.
    void MakeWarningsFatal() {
        fatal_warnings_ = true;
    }

    //! @copydoc Messages::setLogger
    template<typename LogDevice, typename ErrDevice>
    void SetMonitor(Monitor<LogDevice, ErrDevice>* monitor) {
        pMessenger->SetMonitor(monitor);
    }

    //! Delete and free memory allocated per thread in multithreaded applications
    /*!
     * Delete the memory allocated per thread by Cantera.  It should be called
     * from within the thread just before the thread terminates.  If your
     * version of Cantera has not been specifically compiled for thread safety
     * this function does nothing.
     */
    void ThreadComplete();

protected:
    //! Set the default directories for input files.
    /*!
     * %Cantera searches for input files along a path that includes platform-
     * specific default locations, and possibly user-specified locations.
     * This function installs the platform-specific directories on the search
     * path. It is invoked at startup by appinit(), and never should need to
     * be called by user programs.
     *
     * The current directory (".") is always searched first. Then, on Windows, the
     * registry is checked to find the Cantera installation directory, and the
     * 'data' subdirectory of the installation directory will be added to the search
     * path.
     *
     * On any platform, if environment variable CANTERA_DATA is set to a directory
     * name or a list of directory names separated with the OS-dependent path
     * separator (that is, ";" on Windows, ":" elsewhere), then these directories will
     * be added to the search path.
     *
     * Finally, the location where the data files were installed when
     * %Cantera was built is added to the search path.
     *
     * Additional directories may be added by calling function addDirectory.
     * @ingroup inputfiles
     */
    void SetDefaultDirectories();

    //! Current vector of input directories to search for input files
    std::vector<std::string> inputDirs;

    //! Versions of Python to consider when attempting to load user extensions
    vector<string> python_versions_ = {"3.11", "3.10", "3.9", "3.8"};

    //! Vector of deprecation warnings that have been emitted (to suppress
    //! duplicates)
    std::set<std::string> warnings_;

    bool suppress_deprecation_warnings_ = false;
    bool fatal_deprecation_warnings_ = false;
    bool suppress_warnings_ = false;
    bool fatal_warnings_ = false;

    std::set<std::pair<std::string, std::string>> loaded_extensions_;

    ThreadMessage pMessenger_;

private:
    //! Pointer to the single Application instance
    static Application* myapp_;

    //! Pointer to the single MonitorMap instance
    static MonitorMap* mymonitor_;
};

#endif  // SRC_APPLICATION_H
