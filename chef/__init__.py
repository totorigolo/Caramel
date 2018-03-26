import datetime

try:
    from termcolor import colored
except ModuleNotFoundError:
    from sys import stderr
    print('You should install `termcolor` to have the colored console output.', file=stderr)

    # noinspection PyUnusedLocal
    def colored(text, *args, **kwargs):
        return text

PATHS = {
    'java-grammar': 'build/java-grammar',
    'cpp-grammar': 'build/cpp-grammar',
    'grammar-file': 'grammar/Caramel.g4',
    'grammar-test-dir': 'tests/grammar',
    'semantic-test-dir': 'tests/semantic',
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
