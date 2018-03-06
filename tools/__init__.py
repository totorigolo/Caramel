import datetime

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
        '-cp .:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:build/grammar:$CLASSPATH',
        'org.antlr.v4.gui.TestRig',
    ])
}


def seconds_to_string(seconds: float):
    return str(datetime.timedelta(seconds=seconds))
