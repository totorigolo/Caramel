import shlex, subprocess
import os, os.path
import sys

if __name__ == '__main__':

    config = {
        'antlr4': 'java -Xmx500M -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.Tool',
        'javac': 'javac -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH"',
        'grun': 'java -cp ".:../../lib/antlr-lastest-complete.jar:../../lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
    }

    path = "build/grammar"

    print(config["antlr4"])

    subprocess.call(shlex.split("{antlr4} grammar/Caramel.g4 -o build".format(**config)))
    subprocess.call(
        shlex.split(config['javac']) +
        [os.path.join(path, file) for file in os.listdir(path) if
         os.path.isfile(os.path.join(path, file)) and file.endswith(".java")
         ])
    subprocess.call(shlex.split('cp grammar/Caramel.g4 build/grammar/Caramel.g4'))
    os.chdir("build/grammar")

    # call(
    #     '{grun} Caramel r -tree ../../tests/grammar/invalid/simple_declaration.c'.format(**config).split()
    # )

    # call('{grun} Caramel r -tree ../../tests/grammar/valid/simple_declaration.c'.format(**config).split())

    test = {}
    test['valids'] = {}
    test['invalids'] = {}

    validTestFolder = "../../tests/grammar/valid"
    invalidTestFolder = "../../tests/grammar/invalid"

    for testFile in os.listdir(validTestFolder):
        if os.path.isfile(os.path.join(validTestFolder, testFile)):
            with subprocess.Popen(
                    shlex.split('{grun} Caramel r -tree {}/{}'.format(validTestFolder, testFile, **config)),
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
            ) as testProcess:
                if len(testProcess.stderr.read()) == 0:
                    test["valids"][testFile] = True
                else:
                    test["valids"][testFile] = False

    for testFile in os.listdir(invalidTestFolder):
        if os.path.isfile(os.path.join(invalidTestFolder, testFile)):
            with subprocess.Popen(
                    shlex.split('{grun} Caramel r -tree {}/{}'.format(invalidTestFolder, testFile, **config)),
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
            ) as testProcess:
                if len(testProcess.stderr.read()) == 0:
                    test["invalids"][testFile] = False
                else:
                    test["invalids"][testFile] = True

    print(test)
