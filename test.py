import logging
import argparse
import os
import sys
import subprocess

from pathlib import Path

from des_utils import BuildLogger, BuildPaths

"""
Create the command line argument parser

:returns An argument parser to handle the command line arguments
:rtype argparse.ArgumentParser
"""
def create_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description = "DESim - A C++ Discrete Event Simulation framework",
        allow_abbrev = False)

    # Don't use verbose output
    parser.add_argument(
        "--brief",
        action = "store_true",
        help = "Do not use verbose output")

    return parser

"""
Invoke the tests

:param brief  True if verbose output should not be used, false for verbose output
:returns Exit code of CTest
"""
def invoke_tests(brief = False):
    # Run ctest
    ctest_cmd = ["ctest"]
    if ~brief:
        ctest_cmd.append("--verbose")

    proc = subprocess.run(
        ctest_cmd,
        stdout = sys.stdout,
        stderr = sys.stderr,
        text = True)
    
    return proc.returncode
    

# Start build
exit_code = -1

try:
    # Create logger and argument parser
    logger = BuildLogger.create_logger()
    parser = create_arg_parser()

    # Parse command line arguments
    (args, unknown_args) = parser.parse_known_args()

    # Invoke CMake and build project
    try:
        verbose = True
        if args.brief:
            verbose = False

        logger.info("Running tests...")
        os.chdir(BuildPaths.BUILD_DIR)
        result = invoke_tests(verbose)
        if result != 0:
            logger.warning("Testing was not successful")

    except:
        logger.error("Testing failed")
        raise

    finally:
        os.chdir(BuildPaths.BASE_DIR)

    # Build succeeded
    logger.info("Testing finished")
    exit_code = 0
    pass

# Exception occurred while building
except BaseException as ex:
    logger.critical("Testing failed with exception")
    logger.exception(ex)


# Exit with exit_code
finally:
    logger.info(f"Exiting with code {exit_code}")

    sys.stderr.flush()
    sys.stdout.flush()
    sys.exit(exit_code)
