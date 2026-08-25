add_library(glad STATIC)
target_sources(glad
  PUBLIC
    FILE_SET includes
    TYPE HEADERS
    BASE_DIRS
      include
  PRIVATE
    src/glad.c
)
target_compile_features(glad PUBLIC c_std_99)
set_target_properties(glad PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)