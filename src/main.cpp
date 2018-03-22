/*
 * MIT License
 *
 * Copyright (c) 2018 insa.4if.hexanome_kalate
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "datastructure/context/Context.h"

#include "Common.h"
#include "Config.h"
#include "Logger.h"
#include "Console.h"
#include "FrontEnd.h"

#include "tclap.h"

#include <iostream>


Config parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
    Config config{parseArgs(argc, argv)};

    // Get the AST from the front-end
    auto astRoot{Caramel::frontEnd(config)};

    return EXIT_SUCCESS;
}

Config parseArgs(int argc, const char *argv[]) {
    try {
        TCLAP::CmdLine cmd("Caramel - the sweet C compiler", ' ', Caramel_VERSION);

        // Good defaults flag
        // Not enabled by default to respect this project subject
        TCLAP::SwitchArg goodDefaultsArg("b", "good-defaults", "Use good defaults");
        cmd.add(goodDefaultsArg);

        // Static analysis flag
        TCLAP::SwitchArg staticAnalysisArg("a", "static-analysis", "Run static analysis");
        cmd.add(staticAnalysisArg);

        // Optimization flag
        TCLAP::SwitchArg optimizeArg("O", "optimize", "Generate optimized code");
        cmd.add(optimizeArg);

        // Optimization flag
        TCLAP::SwitchArg syntaxTreeDotArg("", "syntax-tree-dot", "Generate a DOT of the syntax tree");
        cmd.add(syntaxTreeDotArg);

        // Compile flag
        TCLAP::SwitchArg compileArg("c", "compile", "Generate assembly code");
        cmd.add(compileArg);

//        // Verbosity flag TODO: A logger
//        TCLAP::MultiSwitchArg verboseArg("v", "verbose", "Set the verbosity level");
//        cmd.add(verboseArg);

        // Source file
        TCLAP::UnlabeledValueArg<string> sourceFileArg("source-file", "The source file", true,
                                                       {}, "string");
        cmd.add(sourceFileArg);

        // Parse the args
        cmd.parse(argc, argv);

        // Create the config from the parsed args
        Config config{};
        if (goodDefaultsArg.getValue()) {
            config.staticAnalysis = true;
            config.optimize = true;
            config.compile = true;
            config.syntaxTreeDot = false;
        } else {
            config.staticAnalysis = staticAnalysisArg.getValue();
            config.optimize = optimizeArg.getValue();
            config.compile = compileArg.getValue();
            config.syntaxTreeDot = syntaxTreeDotArg.getValue();
        }
        config.sourceFile = sourceFileArg.getValue();

        return config;

    } catch (TCLAP::ArgException &e) {
        using namespace Caramel::Colors;
        logger.fatal() << "Error during command line parsing: " << reset
                       << e.error() << " for arg " << e.argId();
        exit(1);
    }
}
