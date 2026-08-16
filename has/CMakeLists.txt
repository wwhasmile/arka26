add_library(has STATIC)
target_sources(has
  PUBLIC
    FILE_SET includes
    TYPE HEADERS
    BASE_DIRS
      src
    FILES
      src/defines.h
      src/version.h
      src/ds/stack.h
      src/version.h
  PRIVATE
    src/lexer.c
    src/ds/stack.c
)
target_compile_features(has PUBLIC c_std_99)
set_target_properties(has PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)
if(
  (CMAKE_C_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
  target_compile_options(has PRIVATE /W3 /WX)
  target_compile_definitions(has PRIVATE _CRT_SECURE_NO_WARNINGS UNICODE)
elseif(
  (CMAKE_C_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_C_COMPILER_ID MATCHES "Clang")
)
  target_compile_options(has PRIVATE -Wall -Wextra -Wpedantic -Werror
    "$<$<CONFIG:Debug>:-g3>" "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>")
endif()

add_library(Arka26::Has ALIAS has)
