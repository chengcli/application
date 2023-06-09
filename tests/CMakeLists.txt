## set up test cases ##

string(TOLOWER ${CMAKE_BUILD_TYPE} buildl)
string(TOUPPER ${CMAKE_BUILD_TYPE} buildu)

# 1. Compile 01_enter_leave.cpp

add_executable(01_enter_leave.${buildl} 01_enter_leave.cpp)

set_target_properties(
  01_enter_leave.${buildl}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
             COMPILE_FLAGS ${CMAKE_CXX_FLAGS_${buildu}})

target_include_directories(
  01_enter_leave.${buildl}
  PRIVATE
  ${APPLICATION_INCLUDE_DIR}
  )

target_link_libraries(01_enter_leave.${buildl}
  application_${buildl}
  )
