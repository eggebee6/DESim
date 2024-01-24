cmake_minimum_required (VERSION 3.14)

set (SRCS_DEMO
  main.cpp
)
  
add_executable (fileIO
  ${SRCS_DEMO}
)

set_target_properties (fileIO
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_EXAMPLES_OUTPUT_DIR}/FileIO"
)

target_include_directories (fileIO
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (fileIO
  PRIVATE
    des
)
