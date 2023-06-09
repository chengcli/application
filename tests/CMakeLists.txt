## set up test cases ##

string(TOLOWER ${CMAKE_BUILD_TYPE} buildl)
string(TOUPPER ${CMAKE_BUILD_TYPE} buildu)

file(GLOB src_files *.cpp)

foreach(test ${src_files})
    get_filename_component(name ${test} NAME_WE)
    add_executable(${name}.${buildl} ${name}.cpp)
    set_target_properties(
      ${name}.${buildl}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                 COMPILE_FLAGS ${CMAKE_CXX_FLAGS_${buildu}})

    target_include_directories(
      ${name}.${buildl}
      PRIVATE
      ${APPLICATION_INCLUDE_DIR}
      )

    target_link_libraries(${name}.${buildl}
      application_${buildl}
      )
endforeach()
