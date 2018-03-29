# coding: utf-8
# MIT License
#
# Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from tools import colored
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
