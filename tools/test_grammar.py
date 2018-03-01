import shlex, subprocess
import os, os.path


def run_tests(openOnFault, dir, result, name, is_expected, config):
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
                    if openOnFault:
                        subprocess.Popen(
                            shlex.split('{grun} Caramel r -gui {}/{}'.format(dir, testFile, **config)),
                            stderr=subprocess.PIPE
                        )


if __name__ == '__main__':

    config = {
        'antlr4': 'java -Xmx500M -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.Tool',
        'javac': 'javac -cp ".:lib/antlr-lastest-complete.jar:lib/antlr-lastest-runtime.jar:$CLASSPATH"',
        'grun': 'java -cp ".:../../lib/antlr-lastest-complete.jar:../../lib/antlr-lastest-runtime.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
    }

    os.system("python tools/compile.py")
    print("Running tests...")

    validTestsName = 'valids'
    validTestFolder = "../../tests/grammar/valid"
    invalidTestName = 'invalids'
    invalidTestFolder = "../../tests/grammar/invalid"
    os.chdir("build/grammar")

    tests = {}
    openOnFault = 'y' == input("Open a gui when errors occur (y,N)? ").lower()
    run_tests(openOnFault, validTestFolder, tests, validTestsName, True, config)
    run_tests(openOnFault, invalidTestFolder, tests, invalidTestName, False, config)

    print("Results (empty means all tests passed): ")

    validTests = tests[validTestsName]
    for key in validTests.keys():
        if not validTests[key]:
            print('Valid test failed', key)

    invalidTests = tests[invalidTestName]
    for key in invalidTests.keys():
        if not invalidTests[key]:
            print('Invalid test has not failed', key)

