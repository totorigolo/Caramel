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

try:
    from termcolor import colored
except ModuleNotFoundError:
    from sys import stderr
    print('You should install `termcolor` to have the colored console output.', file=stderr)
    # noinspection PyUnusedLocal
    def colored(text, *args, **kwargs):
        return text

from tools.logger import trace, logger
import subprocess
import datetime
import shlex
import os

PATHS = {
    'java-grammar': 'build/java-grammar',
    'cpp-grammar': 'build/cpp-grammar',
    'grammar-file': 'grammar/Caramel.g4',
    'grammar-test-dir': 'tests/grammar',
    'semantic-test-dir': 'tests/semantic',
    'backend-test-dir': 'tests/back-end',
    'programs-test-dir': 'tests/programs',
}

COMMANDS = {
    'antlr4': ' '.join([
        'java -Xmx500M',
        '-cp .:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH',
        'org.antlr.v4.Tool',
    ]),
    'javac': ' '.join([
        'javac',
        '-cp .:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH',
    ]),
    'grun': ' '.join([
        'java',
        '-cp .:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:{build_grammar}:$CLASSPATH',
        'org.antlr.v4.gui.TestRig',
    ]),
}

INJECTED_TOKENS_SEPARATOR = '// InjectedTokens'


def seconds_to_string(seconds: float):
    return str(datetime.timedelta(seconds=seconds))


@trace
def exec_(command: str):
    subprocess.run(shlex.split(command))


@trace
def mkdir_and_cd(path: str):
    if not os.path.exists(path):
        os.mkdir(path)
    elif os.path.isfile(path):
        logger.fatal('{} is a file, but should be a directory. Aborting.'.format(path))
        exit(1)
    os.chdir(path)
