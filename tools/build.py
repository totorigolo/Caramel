from tools.logger import logger
from tools.logger import trace
from tools import seconds_to_string
from tools import COMMANDS, PATHS, INJECTED_TOKENS_SEPARATOR
from termcolor import colored
from time import time
import subprocess
import shlex
import re
import os


def _info_completed_in_seconds(seconds: float):
    logger.info('Completed in {}.'.format(
        colored(seconds_to_string(seconds), color='yellow')))


@trace
def get_latest_grammar_file():
    files = []
    grammar_filename_regex = re.compile('Caramel_(v[0-9]+)\.g4')
    for file in os.listdir('grammar/'):
        match = grammar_filename_regex.fullmatch(file)
        if match is not None:
            files.append(file)
    if len(files) == 0:
        logger.critical('There is no grammar file in grammar/.')
        exit(1)
    return sorted(files)[-1]


@trace
def brew_grammar_file(grammar_file_name):
    def strip_token(token_line):
        escaped = False
        result = ''
        for c in token_line.strip():
            if c == ' ' or c == '\t':
                if escaped:
                    result += c
            else:
                if c == "'":
                    escaped = not escaped
                result += c
        return result

    # Determine if the grammar has to be re-brewed
    grammar_time = -2
    if os.path.exists(os.path.join(PATHS['grammar-file'])):
        grammar_time = os.path.getmtime(PATHS['grammar-file'])
    brew_pass_time = -1
    if os.path.exists(os.path.join('grammar/', grammar_file_name)):
        brew_pass_time = os.path.getmtime(os.path.join('grammar/', grammar_file_name))

    if brew_pass_time > grammar_time:
        with open("grammar/" + grammar_file_name, 'r') as grammar_file:
            grammar = grammar_file.read()

            splitted_grammar = grammar.split(INJECTED_TOKENS_SEPARATOR)
            if len(splitted_grammar) == 2:
                tokens_array = splitted_grammar[1].split(';\n')
                tokens = {}
                for token in tokens_array:
                    if len(token) != 0:
                        stripped_token = strip_token(token)
                        token_key, token_value = stripped_token.split(':')
                        tokens[token_key] = token_value

                grammar = splitted_grammar[0]
                old_grammar = grammar[::]
                while True:
                    for token in tokens:
                        grammar = re.sub(token, tokens[token], grammar)
                    if grammar == old_grammar:
                        break
                    old_grammar = grammar[::]

            with open("grammar/Caramel.g4", 'w') as generated_grammar_file:
                generated_grammar_file.write(
                    '''// AUTO-GENERATED FILE - DO NOT EDIT\n''')
                generated_grammar_file.write(grammar)
        logger.info('Grammar file brewed.')
    else:
        logger.info('The brewed grammar is up-to-date.')


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
def build_grammar(args):
    if args.brew:
        # TODO filename = args.filename if args.filename is not None else get_latest_grammar_file()
        filename = get_latest_grammar_file()
        brew_grammar_file(filename)
    if args.language == 'java':
        build_grammar_java()
    elif args.language == 'cpp':
        build_grammar_cpp()
    else:
        logger.fatal('Wrong language:', args.language)
        exit(1)


def build(args):
    logger.info('Build started...')
    start_time = time()

    if args.brew and not (args.grammar or args.all):
        logger.warn("Grammar won't be brewed since it's not built.")

    if args.all:
        args.grammar = True

    if args.grammar:
        build_grammar(args)

    total_time = time() - start_time
    logger.info('Build finished. Total time: {}.'.format(
                colored(seconds_to_string(total_time), color='yellow')))


print(repr(get_latest_grammar_file()))
