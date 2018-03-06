from tools.logger import logger
from tools.logger import trace
from tools import COMMANDS, seconds_to_string
from termcolor import colored
from copy import copy
from time import time
import subprocess
import shlex
import os


class Test:
    def execute(self):
        raise NotImplementedError


class GrammarTest(Test):
    def __init__(self, name: str, full_path, should_fail: bool):
        self.name = name
        self.full_path = full_path
        self.should_fail = should_fail
        self.state = None
        self.succeeded = None

        self.display_name = '{} [{}]'.format(self.name, '-' if self.should_fail else '+')

    @trace
    def execute(self, open_gui=False, open_gui_on_failure=False, show_stdout=False, show_stderr=False):
        start_time = time()
        grun_mode = '-gui' if open_gui else '-tree'
        command = shlex.split('{} Caramel r {} {}'.format(COMMANDS['grun'], grun_mode, self.full_path))
        logger.trace('Test command:', ' '.join(command))
        if len(self.full_path) == 0:  # Interactive test
            print('Enter grammar test input: (ended by ^D)')
        with subprocess.Popen(
                command,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
        ) as test_process:

            # Get stdout and stderr
            out_str = list(map(lambda s: s.decode("utf-8"), test_process.stdout.readlines()))
            error_str = list(map(lambda s: s.decode("utf-8"), test_process.stderr.readlines()))

            # Save the test state
            self.state = {
                'stdout': sum(len(line.strip()) for line in out_str),
                'stderr': sum(len(line.strip()) for line in error_str),
                'time': time() - start_time
            }

            # Determine if unexpected errors, or successes, occured
            errors = self.state['stderr'] > 0
            self.succeeded = not errors or self.should_fail

            # Feed our user
            if self.succeeded:
                logger.info(
                    'Test {}'.format(self.display_name),
                    colored('succeeded.', color='green', attrs=['bold']),
                    colored('[%s]' % seconds_to_string(self.state['time']), color='yellow')
                )
            else:
                logger.info(
                    'Test {}'.format(self.display_name),
                    colored('failed.', color='red', attrs=['bold']),
                    colored('[%s]' % seconds_to_string(self.state['time']), color='yellow')
                )
                if open_gui_on_failure and not open_gui:
                    self.execute(open_gui=True, open_gui_on_failure=False)

            # Show stdout or stderr if asked
            if show_stdout or open_gui:
                if self.state['stdout'] == 0:
                    print(colored('No stdout output.', attrs=['bold']))
                else:
                    print('\n'.join([
                        '#' * 20,
                        colored('stdout output:', attrs=['bold']),
                        *out_str,
                        '-' * 20,
                    ]))
            if show_stderr or open_gui:
                if self.state['stderr'] == 0:
                    print(colored('No stderr output.', attrs=['bold']))
                else:
                    print('\n'.join([
                        '#' * 20,
                        colored('stderr output:', attrs=['bold']),
                        *error_str,
                        '-' * 20,
                    ]))


class Tests:
    def check_build(self):
        raise NotImplementedError

    def discover(self, base_directory):
        raise NotImplementedError


class GrammarTests(Tests):
    def __init__(self):
        self.tests = []

    @trace
    def check_build(self):
        if not os.path.isdir('build'):
            logger.critical(colored(' '.join([
                'The build directory is absent! Try to build before',
                'running tests, or to add --build to the command.'
            ]), color='red'))
            exit(1)

    @trace
    def add_test(self, name: str, full_path, should_fail: bool):
        self.tests.append(GrammarTest(name, full_path, should_fail))
        logger.debug('Added test {}.'.format(name))

    @trace
    def discover(self, base_directory, only=None):
        # Check if there are tests
        if not os.path.isdir('tests/grammar'):
            logger.critical('The tests/grammar directory is absent!')
            exit(1)

        # Discover the tests
        nb_tests_before = len(self.tests)
        for test_directory in sorted(os.listdir(base_directory)):
            logger.debug('Looking for tests in:', test_directory)
            if os.path.isdir(os.path.join(base_directory, test_directory)):
                for test_file in sorted(os.listdir(os.path.join(base_directory, test_directory))):
                    if only is None or test_file in only:
                        test_path = os.path.join(base_directory, test_directory, test_file)
                        if os.path.isfile(test_path):
                            self.add_test(test_file, test_path, test_directory == 'invalid')
        logger.info('Discovered', colored(
            '{} tests.'.format(len(self.tests) - nb_tests_before), color='yellow'))

    @trace
    def run_all(self, *args, **kwargs):
        logger.trace_current()
        for test in self.tests:
            test.execute(*args, **kwargs)


@trace
def test_grammar(args):
    logger.info('Running grammar tests...')

    # Build the grammar, if asked to
    if args.build:
        from tools.build import build_grammar
        build_grammar()

    if args.interactive and len(args.test_files) > 0:
        logger.warn('Running in interactive mode, ignoring test files.')
        args.test_files = []

    # Run the tests
    grammar_tests = GrammarTests()
    grammar_tests.check_build()
    if args.interactive:
        grammar_tests.add_test('interactive test', '', False)
    elif args.all:
        grammar_tests.discover('tests/grammar')
    else:
        grammar_tests.discover('tests/grammar', only=args.test_files)
    grammar_tests.run_all(
        open_gui=args.gui,
        open_gui_on_failure=args.gui_on_failure,
        show_stdout=args.stdout,
        show_stderr=args.stderr,
    )


@trace
def test_all(args):
    logger.info('Running all tests...')

    # Execute grammar tests
    grammar_args = copy(args)
    grammar_args.all = True
    grammar_args.gui = False
    grammar_args.gui_on_failure = False
    test_grammar(grammar_args)
