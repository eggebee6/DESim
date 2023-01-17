cmake_minimum_required (VERSION 3.14)

set (SRCS_DEMO
  Ping.cpp
  main.cpp
)
  
add_executable (pingDemo
  ${SRCS_DEMO}
)

set_target_properties (pingDemo
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_EXAMPLES_OUTPUT_DIR}/Ping"
)

target_include_directories (pingDemo
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (pingDemo
  PRIVATE
    des
)
