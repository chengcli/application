# define default parameters

macro(set_if_empty _variable)
  if("${${_variable}}" STREQUAL "")
    set(${_variable} ${ARGN})
  endif()
endmacro()

# populate the default values
set_if_empty(WINDOWS_SYSTEM "NOT_WINDOWS")

set_if_empty(MYPATH "")
