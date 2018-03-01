import shlex, subprocess
import os, os.path
import sys


def compile_grammar():
    config = {
        'antlr4': 'java -Xmx500M -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.Tool',
        'javac': 'javac -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH"',
        'grun': 'java -cp ".:../../lib/antlr-lastest-complete.jar:../../lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
    }

    path = "build/grammar"

    print("Grammar compilation: ", end='')
    subprocess.call(shlex.split("{antlr4} grammar/Caramel.g4 -o build".format(**config)))
    print("done")
    print("Java compilation: ", end='')
    subprocess.call(
        shlex.split(config['javac']) +
        [os.path.join(path, file) for file in os.listdir(path) if
         os.path.isfile(os.path.join(path, file)) and file.endswith(".java")
         ])
    print("done")


if __name__ == '__main__':
    compile_grammar()
