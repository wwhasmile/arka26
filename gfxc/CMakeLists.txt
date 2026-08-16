add_executable(gfxc)
target_sources(gfxc
  PRIVATE
      src/defs/macros.c
      src/defs/symbols.c
      src/defs/attributes.c
      src/defs/instructions.c
      src/steps/parser.c
      src/steps/analyzer.c
      src/steps/generator.c
      src/gfxc.c
  PRIVATE
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS
      src
)
target_compile_features(gfxc PUBLIC c_std_99)
set_target_properties(gfxc PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)
if(
  (CMAKE_C_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
  target_compile_options(gfxc PRIVATE /W3 /WX)
  target_compile_definitions(gfxc PRIVATE _CRT_SECURE_NO_WARNINGS UNICODE)
elseif(
  (CMAKE_C_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_C_COMPILER_ID MATCHES "Clang")
)
  target_compile_options(gfxc PRIVATE
    -Wall -Wextra -Wpedantic -Werror
    -fno-strict-aliasing
    "$<$<CONFIG:Debug>:-g3>" "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>"
  )
endif()
target_compile_definitions(gfxc PRIVATE
  "$<$<CONFIG:Debug>:VERSION=\"${PROJECT_VERSION}-debug\">"
  "$<$<CONFIG:Release>:VERSION=\"${PROJECT_VERSION}\">"
  "$<$<CONFIG:Debug>:GFXC_DEBUG>"
  "$<$<CONFIG:Release>:GFXC_RELEASE>"
)
target_link_libraries(gfxc PRIVATE Arka26::Has)

add_executable(Arka26::GFXC ALIAS gfxc)
