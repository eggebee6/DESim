cmake_minimum_required (VERSION 3.14)

set (SRCS_TEST
  testJSONEventReader.cpp
  testJSONEventWriter.cpp
)
  
add_executable (testIO
  ${SRCS_TEST}
)

set_target_properties (testIO
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR}
)

target_include_directories (testIO
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (testIO
  PRIVATE
    ${PROJECT_COVERAGE_LIBS}
    nlohmann_json::nlohmann_json
    des
    GTest::gtest_main
)

add_test (NAME IO
  COMMAND testIO
)
