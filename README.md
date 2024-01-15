# DESim
### A C++11 Discrete Event Simulation framework

## Requirements
* A toolchain with C++11 support
* [CMake](https://cmake.org/)
* Make
* Python (for the build and test scripts)

#### Optional
* [Doxygen](http://www.doxygen.nl/) for documentation
* LCOV for coverage (Linux)

## Dependencies
The following are used in the project:
* [Google Test](https://github.com/google/googletest)
* [JSON for Modern C++](https://github.com/nlohmann/json)

Binaries, libraries, documentation, logs, etc. are placed in the __output__ subdirectories

__build.py__ and __test.py__ are provided to automate the build and test processes

##### Build arguments
* _clean_:  Clean the project build directories
* _debug_:  Build with debug symbols
* _document_:  Generate documentation
* _coverage_:  Generate coverage info
* _examples_:  Build examples
* _notest_:  Do not build tests

##### Examples
Basic build
> python build.py

Run tests
> python test.py

Clean build
> python build.py --clean

Build and generate documentation
> python build.py --document

## Development
Active work should merged into the `dev` branch, preferably through a pull request with appropriate review.  Adding unit testing, CI/CD, examples, and **improving documentation** would be fantastic.  The `main` branch should be reserved for clean, tested code.

## Miscellaneous
Not a serious project yet, but feel free to help make it one.
