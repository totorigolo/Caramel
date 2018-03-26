from chef.logger import LoggerLevel
from chef.logger import logger
from chef import seconds_to_string
import chef.build
import chef.clean
import chef.test
from chef import colored
from time import time
import argparse


def _chef():
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
    subparsers = parser.add_subparsers(title='Available commands', dest='subcommand_name')

    # create the parser for the "clean" command
    parser_clean = subparsers.add_parser('clean', help='Ask the Chef to clean up his workplace.')
    parser_clean.set_defaults(func=chef.clean.clean)

    # create the parser for the "build" command
    parser_build = subparsers.add_parser('build', help='Make the Chef cook some Caramel.')
    parser_build.add_argument('-b', '--brew', help='brew the grammar file', action='store_true')
    parser_build.add_argument('-g', '--grammar', help='build the grammar', action='store_true')
    parser_build.add_argument('-c', '--caramel', help='build the compiler', action='store_true')
    parser_build.add_argument('-a', '--all', help='build everything', action='store_true')
    parser_build.set_defaults(func=chef.build.build)

    # create the parser for the "test" command
    parser_test = subparsers.add_parser('test', help='Test the Caramel quality.')

    # create the "test" command common arguments
    def test_common(sub_test_parser: argparse.ArgumentParser):
        sub_test_parser.add_argument('--build', help='build before running tests', action='store_true')
        sub_test_parser.add_argument('--brew', help='brew the grammar file', action='store_true')
        sub_test_parser.add_argument('--language', '-l',
                                     help='set the interpreter language', default='java', choices=['java', 'cpp'])
        sub_test_parser.add_argument('--stdout', help='show the tests stdout output', action='store_true')
        sub_test_parser.add_argument('--stderr', help='show the tests stderr output', action='store_true')

    # Create the "test" sub-commands
    test_subparsers = parser_test.add_subparsers(title='Available sub-commands')

    # Create the parser for the "test grammar" command
    parser_test_grammar = test_subparsers.add_parser('grammar', help='Test the Caramel grammar.')
    parser_test_grammar.set_defaults(func=chef.test.test_grammar)
    test_common(parser_test_grammar)
    parser_test_grammar.add_argument('--interactive', '-i', help='run a test in interactive mode', action='store_true')
    group_test_grammar_gui = parser_test_grammar.add_mutually_exclusive_group()
    group_test_grammar_gui.add_argument('--gui', help='open a GUI when executing test', action='store_true')
    group_test_grammar_gui.add_argument('--gui-on-failure', help='open a GUI on failed tests', action='store_true')
    parser_test_grammar.add_argument('--all', help='run all tests', action='store_true')
    parser_test_grammar.add_argument('test_files', nargs='*', help='test files to test')

    # Create the parser for the "test semantic" command
    parser_test_semantic = test_subparsers.add_parser('semantic', help='Test the Caramel semantic analysis.')
    parser_test_semantic.set_defaults(func=chef.test.test_semantic)
    test_common(parser_test_semantic)
    parser_test_semantic.add_argument('--interactive', '-i', help='run a test in interactive mode', action='store_true')
    group_test_semantic_gui = parser_test_semantic.add_mutually_exclusive_group()
    group_test_semantic_gui.add_argument('--gui', help='open a GUI when executing test', action='store_true')
    group_test_semantic_gui.add_argument('--gui-on-failure', help='open a GUI on failed tests', action='store_true')
    parser_test_semantic.add_argument('--all', help='run all tests', action='store_true')
    parser_test_semantic.add_argument('test_files', nargs='*', help='test files to test')

    # Create the parser for the "test all" command
    parser_test_all = test_subparsers.add_parser('all', help='Run all tests.')
    parser_test_all.set_defaults(func=chef.test.test_all)
    test_common(parser_test_all)

    # parse the command line and call the appropriate submodule
    args = parser.parse_args()
    logger.level = LoggerLevel(args.verbosity - args.quiet)
    if args.subcommand_name is None:
        logger.warn('You forgot to specify the subcommand. Use -h for help.')
        parser.print_usage()
        exit(1)
    else:
        args.func(args)

    logger.info('Completed in {}.'.format(colored(seconds_to_string(time() - start_time), color='yellow')))


def main():
    try:
        _chef()
    except KeyboardInterrupt:
        logger.warn('Chef was ordered to stop early.')
        exit(1)
    except Exception as e:
        logger.critical(colored('An exception occurred:', 'red', None, ['bold']), e)
        raise e


if __name__ == '__main__':
    main()
