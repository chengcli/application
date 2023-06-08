#include "application.hpp"

//! Mutex for creating singletons within the application object
static std::mutex app_mutex;
static std::mutex monitor_mutex;

Application::Logger::Logger(std::string name) {
    auto iter = Application::mymonitor_.find(name);
    if (iter != Application::mymonitor_.end()) {
      cur_monitor_ = iter->second;
      cur_monitor_->Enter();
    } else {
      std
      throw Application::FatalError("Monitor %s not found.", name);
    }
}

~Application::Logger::~Logger() {
    cur_monitor_->Leave();
}

Application::Application()
{
    // install a default log_writer that writes to standard
    // output / standard error
    setDefaultDirectories();
}

Application* Application::GetInstance()
{
    // RAII
    std::unique_lock<std::mutex> lock(app_mutex);

    if (Application::myapp_ == nullptr) {
        Application::myapp_ = new Application();
    }
    return myapp_;
}

void Application::Destroy()
{
    std::unique_lock<std::mutex> lock(app_mutex);
    std::unique_lock<std::mutex> lock(monitor_mutex);

    if (Application::myapp_ != nullptr) {
        delete Application::myapp_;
        Application::myapp_ = nullptr;
    }

    if (Application::mymonitor_ != nullptr) {
        delete Application::mymonitor_;
        Application::mymonitor_ = nullptr;
    }
}

bool Application::InitMonitorLog(std::string_view mod,
    std::string_view fname)
{
}

void Application::WarnDeprecated(std::string_view method,
                                  const std::string& extra)
{
    if (fatal_deprecation_warnings_) {
        throw FatalError("Deprecated method: %s. %s", method, extra);
    } else if (suppress_deprecation_warnings_ || warnings_.count(method)) {
        return;
    }
    warnings_.insert(method);
    WarnLog("Deprecation", fmt::format("{}: {}", method, extra));
}

void Application::warn(std::string_view warning,
                       std::string_view method,
                       const std::string& extra)
{
    if (fatal_warnings_) {
        throw FatalError("method, extra);
    } else if (suppress_warnings_) {
        return;
    }
    WarnLog(warning, fmt::format("{}: {}", method, extra));
}

void Application::thread_complete()
{
    pMessenger.removeThreadMessages();
}

void Application::setDefaultDirectories()
{
    // always look in the local directory first
    inputDirs.push_back(".");

    // if environment variable CANTERA_DATA is defined, then add it to the
    // search path. CANTERA_DATA may include multiple directory, separated by
    // the OS-dependent path separator (in the same manner as the PATH
    // environment variable).
#ifdef _WIN32
    std::string pathsep = ";";
#else
    std::string pathsep = ":";
#endif

    if (getenv("CANTERA_DATA") != 0) {
        string s = string(getenv("CANTERA_DATA"));
        size_t start = 0;
        size_t end = s.find(pathsep);
        while (end != npos) {
            inputDirs.push_back(s.substr(start, end-start));
            start = end + 1;
            end = s.find(pathsep, start);
        }
        inputDirs.push_back(s.substr(start,end));
    }

#ifdef _WIN32
    // Under Windows, the Cantera setup utility records the installation
    // directory in the registry. Data files are stored in the 'data'
    // subdirectory of the main installation directory.
    std::string installDir;
    readStringRegistryKey("SOFTWARE\\Cantera\\Cantera " CANTERA_SHORT_VERSION,
                          "InstallDir", installDir, "");
    if (installDir != "") {
        inputDirs.push_back(installDir + "data");

        // Scripts for converting mechanisms to YAML are installed in
        // the 'bin' subdirectory. Add that directory to the PYTHONPATH.
        const char* old_pythonpath = getenv("PYTHONPATH");
        std::string pythonpath = "PYTHONPATH=" + installDir + "\\bin";
        if (old_pythonpath) {
            pythonpath += ";";
            pythonpath.append(old_pythonpath);
        }
        _putenv(pythonpath.c_str());
    }

#endif

    // CANTERA_DATA is defined in file config.h. This file is written during the
    // build process (unix), and points to the directory specified by the
    // 'prefix' option to 'configure', or else to /usr/local/cantera.
#ifdef CANTERA_DATA
    string datadir = stripnonprint(string(CANTERA_DATA));
    inputDirs.push_back(datadir);
#endif
}

void Application::addDataDirectory(const std::string& dir)
{
    std::unique_lock<std::mutex> dirLock(dir_mutex);
    if (inputDirs.empty()) {
        setDefaultDirectories();
    }
    string d = stripnonprint(dir);

    // Expand "~/" to user's home directory, if possible
    if (d.find("~/") == 0 || d.find("~\\") == 0) {
        char* home = getenv("HOME"); // POSIX systems
        if (!home) {
            home = getenv("USERPROFILE"); // Windows systems
        }
        if (home) {
            d = home + d.substr(1, npos);
        }
    }

    // Remove any existing entry for this directory
    auto iter = std::find(inputDirs.begin(), inputDirs.end(), d);
    if (iter != inputDirs.end()) {
        inputDirs.erase(iter);
    }

    // Insert this directory at the beginning of the search path
    inputDirs.insert(inputDirs.begin(), d);
}

std::string Application::FindInputFile(const std::string& name)
{
    std::unique_lock<std::mutex> dirLock(dir_mutex);
    string::size_type islash = name.find('/');
    string::size_type ibslash = name.find('\\');
    string::size_type icolon = name.find(':');
    std::vector<string>& dirs = inputDirs;

    // Expand "~/" to user's home directory, if possible
    if (name.find("~/") == 0 || name.find("~\\") == 0) {
        char* home = getenv("HOME"); // POSIX systems
        if (!home) {
            home = getenv("USERPROFILE"); // Windows systems
        }
        if (home) {
            string full_name = home + name.substr(1, npos);
            std::ifstream fin(full_name);
            if (fin) {
                return full_name;
            } else {
                throw ApplicationError("Application::FindInputFile",
                                   "Input file '{}' not found", name);
            }
        }
    }

    // If this is an absolute path, just look for the file there
    if (islash == 0 || ibslash == 0
        || (icolon == 1 && (ibslash == 2 || islash == 2)))
    {
        std::ifstream fin(name);
        if (fin) {
            return name;
        } else {
            throw ApplicationError("Application::findInputFile",
                               "Input file '{}' not found", name);
        }
    }

    // Search the Cantera data directories for the input file, and return
    // the full path if a match is found
    size_t nd = dirs.size();
    for (size_t i = 0; i < nd; i++) {
        string full_name = dirs[i] + "/" + name;
        std::ifstream fin(full_name);
        if (fin) {
            return full_name;
        }
    }
    string msg = "\nInput file " + name + " not found in director";
    msg += (nd == 1 ? "y " : "ies ");
    for (size_t i = 0; i < nd; i++) {
        msg += "\n'" + dirs[i] + "'";
        if (i+1 < nd) {
            msg += ", ";
        }
    }
    msg += "\n\n";
    msg += "To fix this problem, either:\n";
    msg += "    a) move the missing files into the local directory;\n";
    msg += "    b) define environment variable CANTERA_DATA to\n";
    msg += "         point to the directory containing the file.";
    throw CanteraError("Application::findInputFile", msg);
}

void Application::loadExtension(const string& extType, const string& name)
{
    if (!usingSharedLibrary()) {
        throw CanteraError("Application::loadExtension",
            "Loading extensions requires linking to the Cantera shared library\n"
            "rather than the static library");
    }
    if (m_loaded_extensions.count({extType, name})) {
        return;
    }

    if (extType == "python" && !ExtensionManagerFactory::factory().exists("python")) {
        string errors;

        // type of imported symbol: void function with no arguments
        typedef void (loader_t)();

        // Only one Python module can be loaded at a time, and a handle needs to be held
        // to prevent it from being unloaded.
        static std::function<loader_t> loader;
        bool loaded = false;

        for (const auto& py_ver : m_pythonSearchVersions) {
            string py_ver_underscore = ba::replace_all_copy(py_ver, ".", "_");
            try {
                loader = boost::dll::import_alias<loader_t>(
                    "cantera_python" + py_ver_underscore, // library name
                    "registerPythonExtensionManager", // symbol to import
                    // append extensions and prefixes, search normal library path, and
                    // expose all loaded symbols (specifically, those from libpython)
                    boost::dll::load_mode::search_system_folders
                    | boost::dll::load_mode::append_decorations
                    | boost::dll::load_mode::rtld_global
                );
                loader();
                loaded = true;
                break;
            } catch (std::exception& err) {
                errors += fmt::format("\nPython {}: {}\n", py_ver, err.what());
            }
        }
        if (!loaded) {
            throw CanteraError("Application::loadExtension",
                "Error loading Python extension support. Tried the following:{}",
                errors);
        }
    }
    ExtensionManagerFactory::build(extType)->registerRateBuilders(name);
    m_loaded_extensions.insert({extType, name});
}

void Application::searchPythonVersions(const string& versions) {
    ba::split(m_pythonSearchVersions, versions, ba::is_any_of(","));
}

Application* Application::myapp_ = nullptr;
