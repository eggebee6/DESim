cmake_minimum_required (VERSION 3.14)

set (SRCS_TEST
  testEvent.cpp
  testEventQueue.cpp
)
  
add_executable (testCore
  ${SRCS_TEST}
)

set_target_properties (testCore
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR}
)

target_include_directories (testCore
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (testCore
  PRIVATE
    ${PROJECT_COVERAGE_LIBS}
    des
    GTest::gtest_main
    GTest::gmock_main
)

add_test (NAME Core
  COMMAND testCore
)