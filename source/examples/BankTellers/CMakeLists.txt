cmake_minimum_required (VERSION 3.14)

set (SRCS_DEMO
  main.cpp
  src/Bank.cpp
  src/Teller.cpp
  src/Customer.cpp
)
  
add_executable (BankTellers
  ${SRCS_DEMO}
)

set_target_properties (BankTellers
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_EXAMPLES_OUTPUT_DIR}/BankTellers"
)

target_include_directories (BankTellers
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ./include
)

target_link_libraries (BankTellers
  PRIVATE
    des
)
