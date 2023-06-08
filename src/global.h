/**
 * @file global.h
 * This file contains definitions for utility functions and text for modules,
 * inputfiles, logs, textlogs, (see \ref inputfiles, \ref logs, and
 * \ref textlogs).
 *
 * @ingroup utils
 *
 * These functions store some parameters in global storage that are accessible
 * at all times from the calling application. Contains module definitions for
 *     -  inputfiles  (see \ref inputfiles)
 *     -  logs        (see \ref logs)
 *     -  textlogs    (see \ref textlogs)
 */

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_GLOBAL_H
#define CT_GLOBAL_H


/*!
 * @defgroup inputfiles Input File Handling
 *
 * The properties of phases and interfaces are specified in text files. These
 * procedures handle various aspects of reading these files.
 *
 * For input files not specified by an absolute pathname, %Cantera searches
 * for input files along a path that includes platform-specific default
 * locations, and possibly user-specified locations.
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
 *
 * %Cantera input files are written using YAML syntax. For more information on using
 * YAML files in Cantera, see the
 * <a href="https://cantera.org/tutorials/yaml/defining-phases.html">YAML Users' Guide</a>
 * or the <a href="../../../../sphinx/html/yaml/index.html">YAML Input File API Reference</a>.
 *
 * %Cantera provides the `ck2yaml` tool for converting Chemkin-format mechanisms to the
 * YAML format. The scripts `cti2yaml.py` and `ctml2yaml.py` can be used to convert
 * legacy CTI and XML input files (from Cantera 2.6 and earlier) to the YAML format.
 *
 * @{
 */

namespace Global {

std::string find_input_file(const std::string& name);

void add_directory(const std::string& dir);

std::string get_data_directories(const std::string& sep);

void load_extension(const std::string& ext_type, const std::string& name);

void search_python_versions(const string& versions);

bool using_shared_library();

void terminate();

void thread_complete();

string version();

bool debug_mode_enabled();

void writelog_direct(const std::string& msg);

}

#endif
