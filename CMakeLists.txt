cmake_minimum_required (VERSION 3.14)
project (DESim VERSION 0.1)


# Set directory variables
set (PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
set (PROJECT_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
set (PROJECT_CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")
set (PROJECT_EXTERN_DIR "${CMAKE_SOURCE_DIR}/extern")
set (PROJECT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output")

set (PROJECT_RUNTIME_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/bin")
set (PROJECT_ARCHIVE_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/lib")
set (PROJECT_LIBRARY_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/lib")

set (PROJECT_TEST_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/test")
set (PROJECT_TEST_BINARY_DIR "${PROJECT_TEST_OUTPUT_DIR}/bin")

set (PROJECT_EXAMPLES_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/examples")
set (PROJECT_EXAMPLES_BINARY_DIR "${PROJECT_EXAMPLES_OUTPUT_DIR}/bin")

# Set options
option (BUILD_TESTS "Build tests" ON)
option (BUILD_EXAMPLES "Build examples" ON)
option (BUILD_WITH_COVERAGE "Build with coverage" OFF)

message (STATUS "Variable BUILD_TESTS:  ${BUILD_TESTS}")
message (STATUS "Variable BUILD_EXAMPLES:  ${BUILD_EXAMPLES}")
message (STATUS "Variable BUILD_WITH_COVERAGE:  ${BUILD_WITH_COVERAGE}")
message (STATUS "Variable MSVC:  ${MSVC}")

# Set build flags
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED true)

if (MSVC)
  set (PROJECT_COMPILE_OPTIONS "/W3")
  
  # TODO: Coverage on Windows
  if (BUILD_WITH_COVERAGE)
    set (PROJECT_COVERAGE_FLAGS "")
    set (BUILD_WITH_COVERAGE OFF)
    message (WARNING "Coverage not (yet) supported on Windows")
  endif ()

else ()
  set (PROJECT_COMPILE_OPTIONS -Wall)
  set (PROJECT_COVERAGE_FLAGS -fprofile-arcs -ftest-coverage)

endif ()

message (STATUS "Compile options:  ${PROJECT_COMPILE_OPTIONS}")
message (STATUS "Coverage flags:  ${PROJECT_COVERAGE_FLAGS}")

add_compile_options (${PROECT_COMPILE_OPTIONS})

# Enable testing if option is set
if (BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  
  FetchContent_MakeAvailable(googletest)

  enable_testing ()
  
endif ()

# Add source directory
add_subdirectory (${PROJECT_SOURCE_DIR})

# Clear cached variables
unset (BUILD_TESTS CACHE)
unset (BUILD_WITH_COVERAGE CACHE)
unset (BUILD_EXAMPLES CACHE)
