import logging
import argparse
import os
import stat
import sys
import shutil
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

    # Skip building tests
    parser.add_argument(
        "--notest",
        action = "store_true",
        help = "Do not build tests")

    # Build for debug
    parser.add_argument(
        "--debug",
        action = "store_true",
        help = "Build for debugging")

    # Clean build and output paths
    parser.add_argument(
        "--clean",
        action = "store_true",
        help = "Clean build path and output path")

    # Generate documentation
    parser.add_argument(
        "--document",
        action = "store_true",
        help = "Generate documentation after building")

    # Build with coverage
    parser.add_argument(
        "--coverage",
        action = "store_true",
        help = "Build with coverage information")

    # Build example projects
    parser.add_argument(
        "--examples",
        action = "store_true",
        help = "Build examples")

    return parser


"""
Invoke the build system

:param base_path  Path to project base directory
:param cmake_args  List of command line arguments for CMake
"""
def invoke_build(base_path, cmake_args):
    if not base_path:
        raise ValueError("No base path specified")

    if not cmake_args:
        raise ValueError("No CMake argument list provided")

    # TODO: Allow different build systems to be used

    # Run CMake
    cmake_cmd = ["cmake", "-G", "NMake Makefiles"]
    cmake_cmd.extend(cmake_args)
    cmake_cmd.append(str(base_path))

    proc = subprocess.run(
        cmake_cmd,
        stdout = sys.stdout,
        stderr = sys.stderr,
        text = True)

    proc.check_returncode()

    # Run NMake
    nmake_cmd = ["nmake"]

    proc = subprocess.run(
        nmake_cmd,
        stdout = sys.stdout,
        stderr = sys.stderr,
        text = True)

    proc.check_returncode()


# Start build
exit_code = -1

try:
    # Create logger and argument parser
    logger = BuildLogger.create_logger()
    parser = create_arg_parser()

    # Parse command line arguments
    (args, unknown_args) = parser.parse_known_args()

    # Clean build and output paths if requested
    if args.clean:
        try:
            for path in [
                    BuildPaths.BUILD_DIR,
                    BuildPaths.OUTPUT_DIR]:
                logger.info(f"Removing {path}...")
                if path.exists():
                    shutil.rmtree(
                        path,
                        onerror = lambda func, path, _: (os.chmod(path, stat.S_IWRITE), func(path)) )
            # Done
            logger.info("Clean finished")
            exit_code = 0
            pass

        except:
            logger.error("Failed to clean build paths")
            raise

    # No clean requested, proceed to build
    else:
        # Create CMake argument list
        cmake_args = []

        if args.notest:
            cmake_args.append("-DBUILD_TESTS=OFF")
        else:
            cmake_args.append("-DBUILD_TESTS=ON")

        if args.debug:
            cmake_args.append("-DCMAKE_BUILD_TYPE=Debug")
        else:
            cmake_args.append("-DCMAKE_BUILD_TYPE=Release")

        if args.coverage:
            cmake_args.append("-DBUILD_WITH_COVERAGE=ON")

        if args.examples:
            cmake_args.append("-DBUILD_EXAMPLES=ON")


        # Create build paths
        try:
            for path in [
                    BuildPaths.BUILD_DIR,
                    BuildPaths.OUTPUT_DIR,
                    BuildPaths.LOG_DIR,
                    BuildPaths.COVERAGE_DIR,
                    BuildPaths.DOCS_DIR]:
                logger.info(f"Creating {path}...")
                if not path.exists():
                    path.mkdir(parents = True, exist_ok = True)
        except:
            logger.error("Failed to create build paths")
            raise


        # Invoke CMake and build project
        try:
            logger.info("Building...")
            os.chdir(BuildPaths.BUILD_DIR)
            invoke_build(BuildPaths.BASE_DIR, cmake_args)

        except:
            logger.error("Build failed")
            raise

        finally:
            os.chdir(BuildPaths.BASE_DIR)


        # Generate documentation if requested
        if args.document:
            try:
                logger.info("Generating documentation...")

                doxy_cmd = ["doxygen", f"{BuildPaths.DOC_CONFIG_DIR}/Doxyfile"]
                proc = subprocess.run(
                    doxy_cmd,
                    stdout = sys.stdout,
                    stderr = sys.stderr,
                    text = True)

                proc.check_returncode()
                
            except:
                logger.error("Generate documentation failed")
                raise

        # Build succeeded
        logger.info("Build finished")
        exit_code = 0
        pass

# Exception occurred while building
except BaseException as ex:
    logger.critical("Build failed with exception")
    logger.exception(ex)


# Exit with exit_code
finally:
    logger.info(f"Exiting with code {exit_code}")

    sys.stderr.flush()
    sys.stdout.flush()
    sys.exit(exit_code)
