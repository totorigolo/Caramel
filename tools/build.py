from tools.logger import logger
from tools.logger import trace
from tools import seconds_to_string
from tools import COMMANDS
from termcolor import colored
from time import time
import subprocess
import shlex
import os


def _info_completed_in_seconds(seconds: float):
    logger.info('Completed in {}.'.format(
        colored(seconds_to_string(seconds), color='yellow')))


@trace
def build_grammar_cpp():
    # TODO: Test if the .g4 file is more recent than the build files

    path = "build/cpp-grammar"

    start_time = time()
    logger.info('Compiling grammar in C++...')
    subprocess.call(shlex.split("{} grammar/Caramel.g4 -o {} -visitor -listener -Dlanguage=Cpp -Xexact-output-dir"
                                .format(COMMANDS['antlr4'], path)))
    _info_completed_in_seconds(time() - start_time)


@trace
def build_grammar_java():
    # TODO: Test if the .g4 file is more recent than the build files

    path = "build/java-grammar"

    start_time = time()
    logger.info('Compiling grammar in Java...')
    subprocess.call(shlex.split("{} grammar/Caramel.g4 -o {} -visitor -listener -Xexact-output-dir".format(COMMANDS['antlr4'], path)))
    _info_completed_in_seconds(time() - start_time)

    start_time = time()
    logger.info('Compiling generated java sources...')
    subprocess.call(
        shlex.split(COMMANDS['javac']) + [
            os.path.join(path, file) for file in os.listdir(path) if
            os.path.isfile(os.path.join(path, file)) and file.endswith(".java")
        ])
    _info_completed_in_seconds(time() - start_time)


@trace
def build_grammar(language):
    if language == 'java':
        build_grammar_java()
    elif language == 'cpp':
        build_grammar_cpp()
    else:
        logger.fatal('Wrong language:', language)
        exit(1)


def build(args):
    logger.info('Build started...')
    start_time = time()

    if args.all:
        args.grammar = True

    if args.grammar:
        build_grammar(args.language)

    total_time = time() - start_time
    logger.info('Build finished. Total time:',
                colored(seconds_to_string(total_time), color='yellow'), '.')
