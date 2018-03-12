from termcolor import colored
from enum import IntEnum
import inspect
from functools import wraps


class LoggerLevel(IntEnum):
    OFF = 1
    FATAL = 2
    WARN = 3
    INFO = 4
    DEBUG = 5
    TRACE = 6


class Logger:
    def __init__(self, level=LoggerLevel.INFO):
        self.level = level

    def log(self, level, *args, **kwargs):
        if self.level >= level:
            print(*args, **kwargs)

    def fatal(self, *args, **kwargs):
        if self.level >= LoggerLevel.FATAL:
            print(colored('[fatal]', color='red', attrs=['bold']), *args, **kwargs)

    def critical(self, *args, **kwargs):
        if self.level >= LoggerLevel.FATAL:
            print(colored('[critical]', color='red', attrs=['bold']), *args, **kwargs)

    def warn(self, *args, **kwargs):
        if self.level >= LoggerLevel.WARN:
            print(colored('[warn]', color='yellow', attrs=['bold']), *args, **kwargs)

    def info(self, *args, **kwargs):
        if self.level >= LoggerLevel.INFO:
            print(colored('[info]', color='blue', attrs=['bold']), *args, **kwargs)

    def debug(self, *args, **kwargs):
        if self.level >= LoggerLevel.DEBUG:
            print(colored('[debug]', color='cyan', attrs=['bold']), *args, **kwargs)

    def trace(self, *message):
        if self.level >= LoggerLevel.TRACE:
            print(colored('[trace]', color='magenta', attrs=['bold']),
                  colored(' '.join(map(str, message)), color='magenta'))

    def trace_current(self):
        if self.level >= LoggerLevel.TRACE:
            func = inspect.stack()[1][3]
            file = inspect.stack()[1][1].split('/')[-1]
            self.trace('{}#{}'.format(file, func))


def trace(func):
    @wraps(func)
    def wrapper(*args, **kw):
        logger.trace('Executing:', func.__name__)
        return func(*args, **kw)
    return wrapper


logger = Logger()
