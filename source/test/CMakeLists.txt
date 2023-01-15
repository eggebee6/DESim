cmake_minimum_required (VERSION 3.0)

set (PROJECT_COVERAGE_LIBS
)

if (BUILD_WITH_COVERAGE)
  add_compile_options (${PROJECT_COVERAGE_FLAGS})
  set (PROJECT_COVERAGE_LIBS
    gcov
  )
endif ()

add_subdirectory (core)
