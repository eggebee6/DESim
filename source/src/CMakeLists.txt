cmake_minimum_required (VERSION 3.14)

# Fetch JSON library
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)

set (SRCS_CORE
  "core/Event.cpp"
  "core/EventQueue.cpp"
  "core/SimEngine.cpp"
)

set (SRCS_IO
  "io/JsonEventReader.cpp"
  "io/JsonEventWriter.cpp"
)

add_library (des
  ${SRCS_CORE}
  ${SRCS_IO}
)

set_target_properties (des
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_LIBRARY_OUTPUT_DIR}
)

target_include_directories (des
  PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries (des
  PRIVATE
    ${PROJECT_COVERAGE_LIBS}
    nlohmann_json::nlohmann_json
)
