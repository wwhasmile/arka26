cmake_minimum_required(VERSION 3.23)

project(arka26 C)

find_package(Git REQUIRED)
if (GIT_EXECUTABLE)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION
    RESULT_VARIABLE ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()
if (PROJECT_VERSION STREQUAL "")
  set(PROJECT_VERSION "local")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(3rdparty)

add_subdirectory(has)
add_subdirectory(gfxc)
add_subdirectory(arka26)
