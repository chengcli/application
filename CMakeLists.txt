# Installs debugger library
#
#   library: libdebugger_debug.o
#   library: libdebugger_release.o
#
# Define the following variables
#
#   DEBUGGER_FOUND
#   DEBUGGER_INCLUDE_DIR
#   DEBUGGER_LIBRARY_DEBUG
#   DEBUGGER_LIBRARY_RELEASE
#
# Normal usage would be:
#
#   include_directories( ${DEBUGGER_INCLUDE_DIR})
#   target_link_libraries( ${DEBUGGER_LIBRARY_DEBUG})
#
cmake_minimum_required(VERSION 3.10)

project(
  debugger
  LANGUAGES CXX
  )

## set up compiler flags ##
message(STATUS "")
message(STATUS "Set up project compiler flags ...")
message(STATUS "Include ${CMAKE_SOURCE_DIR}/cmake/setup_compiler_flags.cmake")
include(${CMAKE_SOURCE_DIR}/cmake/setup_compiler_flags.cmake)

## set up project configuration ##
message(STATUS "Set up project configuration flags ...")
message(STATUS "Include ${CMAKE_SOURCE_DIR}/cmake/setup_configure.cmake")
include(${CMAKE_SOURCE_DIR}/cmake/setup_configure.cmake)

file(GLOB _src_files
  src/*.cpp
  )

foreach(build ${BUILD_TYPES})
  string(TOLOWER ${build} buildl)
  add_library(debugger_${buildl}
    ${_src_files})

  set_target_properties(debugger_${buildl}
    PROPERTIES
    COMPILE_FLAGS ${CMAKE_CXX_FLAGS_${build}}
    )

  set(DEBUGGER_LIBRARY_${build} debugger_${buildl}
    CACHE STRING 
    "debugger library ${build}")
  mark_as_advanced(DEBUGGER_LIBRARY_${build})
endforeach()

set(DEBUGGER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} 
  CACHE PATH 
  "debugger include directory")
set(DEBUGGER_FOUND TRUE CACHE BOOL "FIND DEBUGGER LIBRARY")
mark_as_advanced(DEBUGGER_INCLUDE_DIR)
mark_as_advanced(DEBUGGER_FOUND)
