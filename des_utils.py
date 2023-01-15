import logging
import os

from pathlib import Path

class BuildLogger(object):
    """
    Create the logger

    :returns Logger to be used during the tests
    :rtype logging.Logger
    """
    def create_logger() -> logging.Logger:
        LOGGING_FORMAT = "[%(levelname)s] - %(message)s"    # Log message format
        LOGGER_NAME = "DESIM_LOGGER"                        # Logger name
        LOG_LEVEL = logging.DEBUG                           # Lowest severity level to log

        # Create logger
        logger = logging.getLogger(LOGGER_NAME)
        logger_formatter = logging.Formatter(LOGGING_FORMAT)

        # Add console handler
        logger_console = logging.StreamHandler()
        logger_console.setFormatter(logger_formatter)
        logger.addHandler(logger_console)

        # Set log level  
        logger.setLevel(LOG_LEVEL)

        return logger


# Set paths
class BuildPaths(object):
    BASE_DIR = Path(os.getcwd())

    SOURCE_DIR = BASE_DIR / "source"
    DOC_CONFIG_DIR = BASE_DIR / "docs"

    BUILD_DIR = BASE_DIR / "build"
    OUTPUT_DIR = BASE_DIR / "output"

    LOG_DIR = OUTPUT_DIR / "logs"
    COVERAGE_DIR = OUTPUT_DIR / "coverage"
    DOCS_DIR = OUTPUT_DIR / "documentation"
