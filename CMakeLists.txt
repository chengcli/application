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
cmake_minimum_required(VERSION 3.18)

project(
  debugger
  LANGUAGES CXX
  )

## 1. set up compiler flags ##
message(STATUS "")
message(STATUS "Set up project compiler flags ...")
message(STATUS "Include ${CMAKE_SOURCE_DIR}/cmake/setup_compiler_flags.cmake")
include(${CMAKE_SOURCE_DIR}/cmake/setup_compiler_flags.cmake)

## 2. set up project configuration ##
message(STATUS "Set up project configuration flags ...")
message(STATUS "Include ${CMAKE_SOURCE_DIR}/cmake/setup_configure.cmake")
include(${CMAKE_SOURCE_DIR}/cmake/setup_configure.cmake)

## 3. set up project paths ##
message(STATUS "")
message(STATUS "Set up project paths ...")
set(EXECUTABLE_RELDIR "bin")
set(INCLUDE_RELDIR "include")
set(SOURCE_RELDIR "source")

## 4. set up project system libraries ##
message(STATUS "")
message(STATUS "Set up project system libraries ...")

if (${UseMPI})
  find_package(MPI REQUIRED)
  set(EnableMPI MPI_PARALLEL)
else()
  set(EnableMPI NOT_MPI_PARALLEL)
endif()

## 5. set up project library ##
message(STATUS "")
message(STATUS "Set up project complication and installation ...")

# set up configure file
configure_file(${CMAKE_SOURCE_DIR}/configure.hpp.in configure.hpp @ONLY)

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

  target_include_directories(debugger_${buildl}
    SYSTEM PRIVATE
    ${MPI_CXX_INCLUDE_PATH}
    ${CMAKE_BINARY_DIR}
    )

  target_link_libraries(debugger_${buildl}
    ${MPI_CXX_LIBRARIES}
    )

  set(DEBUGGER_LIBRARY_${build} debugger_${buildl}
    CACHE STRING 
    "debugger library ${build}")
  mark_as_advanced(DEBUGGER_LIBRARY_${build})
endforeach()

set(DEBUGGER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
  CACHE PATH 
  "debugger include directory")
set(DEBUGGER_FOUND TRUE CACHE BOOL "FIND DEBUGGER LIBRARY")
mark_as_advanced(DEBUGGER_INCLUDE_DIR)
mark_as_advanced(DEBUGGER_FOUND)

## 5. set up test cases
add_subdirectory(tests)
