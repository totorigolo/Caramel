import shlex, subprocess
import os, os.path
import sys

def run_tests(dir, result, name, is_expected):
    if name not in result:
        result[name] = {}
    for testFile in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, testFile)):
            with subprocess.Popen(
                    shlex.split('{grun} Caramel r -tree {}/{}'.format(dir, testFile, **config)),
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
            ) as testProcess:
                if len(testProcess.stderr.read()) == 0:
                    tests[name][testFile] = is_expected
                else:
                    tests[name][testFile] = not is_expected


if __name__ == '__main__':

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

    print("Running tests...")

    validTestsName = 'valids'
    validTestFolder = "../../tests/grammar/valid"
    invalidTestName = 'invalids'
    invalidTestFolder = "../../tests/grammar/invalid"
    os.chdir("build/grammar")

    tests = {}
    run_tests(validTestFolder, tests, validTestsName, True)
    run_tests(invalidTestFolder, tests, invalidTestName, False)

    print("Results (empty means all tests passed): ")

    validTests = tests[validTestsName]
    for key in validTests.keys():
        if not validTests[key]:
            print('Valid test failed', key)

    invalidTests = tests[invalidTestName]
    for key in invalidTests.keys():
        if not invalidTests[key]:
            print('Invalid test has not failed', key)

