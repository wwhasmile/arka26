add_executable(arka26)
target_sources(arka26
  PRIVATE
      src/main.c
  PRIVATE
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS
      src
)
target_compile_features(arka26 PUBLIC c_std_99)
set_target_properties(arka26 PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)
if(
  (CMAKE_C_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
  target_compile_options(arka26 PRIVATE /W3 /WX)
  target_compile_definitions(has PRIVATE _CRT_SECURE_NO_WARNINGS UNICODE)
elseif(
  (CMAKE_C_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_C_COMPILER_ID MATCHES "Clang")
)
  target_compile_options(arka26 PRIVATE -Wall -Wextra -Wpedantic -Werror
    "$<$<CONFIG:Debug>:-g3>" "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>"
  )
endif()
target_compile_definitions(arka26 PRIVATE
  "$<$<CONFIG:Debug>:VERSION=\"${PROJECT_VERSION}-debug\">"
  "$<$<CONFIG:Release>:VERSION=\"${PROJECT_VERSION}\">"
  "$<$<CONFIG:Debug>:ARKA26_DEBUG>"
  "$<$<CONFIG:Release>:ARKA26_RELEASE>"
)
target_link_libraries(arka26 PRIVATE Arka26::Has)

add_executable(Arka26::Arka26 ALIAS arka26)
