from tools.logger import LoggerLevel
from tools.logger import logger
from tools import seconds_to_string
import tools.build
import tools.clean
import tools.test
from termcolor import colored
from time import time
import argparse


def chef():
    start_time = time()

    # create the top-level parser
    parser = argparse.ArgumentParser(
        description='The Caramel Jack of all trades.'
    )
    group_verbosity = parser.add_mutually_exclusive_group()
    group_verbosity.add_argument(
        '--verbose', '-v', help='increase the verbosity (repeat for even more verbosity)',
        dest='verbosity', action='count', default=4)
    group_verbosity.add_argument(
        '--quiet', '-q', help='decrease the verbosity (repeat for less verbosity)',
        action='count', default=0)
    subparsers = parser.add_subparsers(title='Available commands')

    # create the parser for the "clean" command
    parser_clean = subparsers.add_parser('clean', help='Ask the Chef to clean up his workplace.')
    parser_clean.set_defaults(func=tools.clean.clean)

    # create the parser for the "build" command
    parser_build = subparsers.add_parser('build', help='Make the Chef cook some Caramel.')
    parser_build.add_argument('--grammar', help='build the grammar', action='store_true')
    parser_build.add_argument('--all', help='build everything', action='store_true')
    parser_build.set_defaults(func=tools.build.build)

    # create the parser for the "test" command
    parser_test = subparsers.add_parser('test', help='Test the Caramel quality.')

    # create the "test" command common arguments
    def test_common(sub_test_parser: argparse.ArgumentParser):
        sub_test_parser.add_argument('--build', help='build before running tests', action='store_true')
        sub_test_parser.add_argument('--stdout', help='show the tests stdout output', action='store_true')
        sub_test_parser.add_argument('--stderr', help='show the tests stderr output', action='store_true')

    # Create the "test" sub-commands
    test_subparsers = parser_test.add_subparsers(title='Available sub-commands')

    # Create the parser for the "test grammar" command
    parser_test_grammar = test_subparsers.add_parser('grammar', help='Test the Caramel grammar.')
    parser_test_grammar.set_defaults(func=tools.test.test_grammar)
    test_common(parser_test_grammar)
    parser_test_grammar.add_argument('--interactive', '-i', help='run a test in interactive mode', action='store_true')
    group_test_grammar_gui = parser_test_grammar.add_mutually_exclusive_group()
    group_test_grammar_gui.add_argument('--gui', help='open a GUI when executing test', action='store_true')
    group_test_grammar_gui.add_argument('--gui-on-failure', help='open a GUI on failed tests', action='store_true')
    parser_test_grammar.add_argument('--all', help='run all tests', action='store_true')
    parser_test_grammar.add_argument('test_files', nargs='*', help='test files to test')

    # Create the parser for the "test all" command
    parser_test_all = test_subparsers.add_parser('all', help='Run all tests.')
    parser_test_all.set_defaults(func=tools.test.test_all)
    test_common(parser_test_all)

    # parse the command line and call the appropriate submodule
    args = parser.parse_args()
    logger.level = LoggerLevel(args.verbosity - args.quiet)
    args.func(args)

    logger.info('Completed in {}.'.format(colored(seconds_to_string(time() - start_time), color='yellow')))


def main():
    try:
        chef()
    except Exception as e:
        logger.critical(colored('An exception occured:', 'red', None, ['bold']), e)
        raise e


if __name__ == '__main__':
    main()
