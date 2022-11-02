## set up test cases ##

file(GLOB _tests *.cpp)
foreach(_test ${_tests})
  foreach(build ${BUILD_TYPES})
    string(TOLOWER ${build} buildl)
    get_filename_component(_name ${_test} NAME_WE)

    add_executable(${_name}.${buildl} ${_test})

    set_target_properties(${_name}.${buildl}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${EXECUTABLE_RELDIR}"
      COMPILE_FLAGS ${CMAKE_CXX_FLAGS_${build}}
      )

    target_include_directories(${_name}.${buildl}
      SYSTEM PRIVATE
      ${DEBUGGER_INCLUDE_DIR}
      )

    target_link_libraries(${_name}.${buildl}
      ${DEBUGGER_LIBRARY_${build}}
      )
  endforeach()
endforeach()
