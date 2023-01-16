cmake_minimum_required (VERSION 3.14)

set (SRCS_CORE
  "core/Event.cpp"
  "core/EventQueue.cpp"
  "core/SimEngine.cpp"
)

add_library (des
  ${SRCS_CORE}
)

set_target_properties (des
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_LIBRARY_OUTPUT_DIR}
)

target_include_directories (des
  PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
