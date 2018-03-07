from tools.logger import logger
from tools.logger import trace
from tools import seconds_to_string
from tools import COMMANDS, PATHS
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

    path = PATHS['cpp-grammar']

    # Determine if the grammar has to be rebuilt
    grammar_time = os.path.getmtime(PATHS['grammar-file'])
    antlr_pass_time = -1
    if os.path.exists(os.path.join(path, 'CaramelParser.cpp')):
        antlr_pass_time = os.path.getmtime(os.path.join(path, 'CaramelParser.cpp'))

    # Build java sources from the grammar
    if grammar_time > antlr_pass_time:
        start_time = time()
        logger.info('Compiling grammar in C++...')
        subprocess.call(shlex.split("{} {} -o {} -visitor -listener -Dlanguage=Cpp -Xexact-output-dir"
                                    .format(COMMANDS['antlr4'], PATHS['grammar-file'], path)))
        _info_completed_in_seconds(time() - start_time)
    else:
        logger.info('The compiled C++ grammar is up-to-date.')


@trace
def build_grammar_java():
    # TODO: Test if the .g4 file is more recent than the build files

    path = PATHS['java-grammar']

    # Determine if the grammar has to be rebuilt
    grammar_time = os.path.getmtime(PATHS['grammar-file'])
    antlr_pass_time = -1
    javac_pass_time = -1
    if os.path.exists(os.path.join(path, 'CaramelParser.java')):
        antlr_pass_time = os.path.getmtime(os.path.join(path, 'CaramelParser.java'))
    if os.path.exists(os.path.join(path, 'CaramelParser.class')):
        javac_pass_time = os.path.getmtime(os.path.join(path, 'CaramelParser.class'))

    # Build java sources from the grammar
    if grammar_time > antlr_pass_time:
        start_time = time()
        logger.info('Compiling grammar in Java...')
        subprocess.call(shlex.split("{} {} -o {} -visitor -listener -Xexact-output-dir".format(
            COMMANDS['antlr4'], PATHS['grammar-file'], path)))
        _info_completed_in_seconds(time() - start_time)
    else:
        logger.info('The compiled java grammar is up-to-date.')

    # Compile the built sources
    if grammar_time > javac_pass_time:
        start_time = time()
        logger.info('Compiling generated java sources...')
        subprocess.call(
            shlex.split(COMMANDS['javac']) + [
                os.path.join(path, file) for file in os.listdir(path) if
                os.path.isfile(os.path.join(path, file)) and file.endswith(".java")
            ])
        _info_completed_in_seconds(time() - start_time)
    else:
        logger.info('The compiled java sources are up-to-date.')


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
