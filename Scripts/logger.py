import logging
import sys

FORMAT = "%(asctime)s - %(levelname)s - %(message)s"

SUCCESS = 25


class Colors:
    grey = "\x1b[0;37m"
    green = "\x1b[1;32m"
    yellow = "\x1b[1;33m"
    red = "\x1b[1;31m"
    purple = "\x1b[1;35m"
    blue = "\x1b[1;34m"
    light_blue = "\x1b[1;36m"
    reset = "\x1b[0m"
    blink_red = "\x1b[5m\x1b[1;31m"


class CustomFormatter(logging.Formatter):

    fmt = FORMAT

    FORMATS = {
        logging.DEBUG: Colors.grey + fmt + Colors.reset,
        logging.INFO: Colors.grey + fmt + Colors.reset,
        logging.WARNING: Colors.yellow + fmt + Colors.reset,
        logging.ERROR: Colors.red + fmt + Colors.reset,
        logging.CRITICAL: Colors.blink_red + fmt + Colors.reset,
        SUCCESS: Colors.green + fmt + Colors.reset,
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)


class CustomLogger(logging.Logger):
    def __init__(self, name, level=logging.NOTSET):
        super().__init__(name, level)
        logging.addLevelName(SUCCESS, "SUCCESS")

    def success(self, msg: str, *args, **kwargs) -> None:
        if self.isEnabledFor(SUCCESS):
            self._log(SUCCESS, msg, args, **kwargs)


def getLogger(name: str, level: int = logging.DEBUG) -> CustomLogger:

    logging.basicConfig(level=level)
    logging.setLoggerClass(CustomLogger)
    logger: CustomLogger = logging.getLogger(name)  # type: ignore

    logger.handlers.clear()

    handler = logging.StreamHandler(stream=sys.stdout)
    handler.setLevel(level)
    handler.setFormatter(CustomFormatter())

    logger.addHandler(handler)
    logger.propagate = False

    return logger


if __name__ == "__main__":
    logger = getLogger(__file__, logging.DEBUG)
    logger.debug("debug message")
    logger.info("info message")
    logger.warning("warning message")
    logger.error("error message")
    logger.critical("critical message")
    logger.success("test")
