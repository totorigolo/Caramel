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

#include "ast/context/Context.h"

#include "Common.h"
#include "Config.h"
#include "Logger.h"
#include "Console.h"
#include "FrontEnd.h"
#include "BackEnd.h"

#include "tclap.h"
#include "ir/x86_64/X86_64CFGVisitor.h"

#include <iostream>


Config parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
    Config config{parseArgs(argc, argv)};

    // Get the AST from the front-end
    Context::Ptr astRoot{caramel::frontEnd(config)};

    // Get the x86_64 assembly from the back-end
    std::stringstream assemblySS;
    caramel::ir::CFGVisitor::Ptr arch = std::shared_ptr<caramel::ir::CFGVisitor>(new caramel::ir::x86_64::X86_64CFGVisitor);
    caramel::BackEnd::generateAssembly(config.sourceFile, astRoot, assemblySS, arch);
    std::string assembly = assemblySS.str();

    // Print the assembly on the standard output
    std::cout << assembly;

    // Compile the assembly on Linux
    // TODO: Dirty, change this.
#ifdef __linux__
    std::ofstream assemblyOutputFile("assembly.s");
    assemblyOutputFile << assembly;
    assemblyOutputFile.close();
    system("gcc assembly.s -o caramel.out");
#else
    logger.info() << "No assembly compilation on Windows.";
#endif

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

        // Syntax tree - DOT export
        TCLAP::SwitchArg syntaxTreeDotArg("", "syntax-tree-dot", "Generate a DOT of the syntax tree");
        cmd.add(syntaxTreeDotArg);

        // AST - DOT export
        TCLAP::SwitchArg astDotArg("", "ast-dot", "Generate a DOT of the AST");
        cmd.add(astDotArg);

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
        config.staticAnalysis = staticAnalysisArg.getValue();
        config.optimize = optimizeArg.getValue();
        config.compile = compileArg.getValue();
        config.syntaxTreeDot = syntaxTreeDotArg.getValue();
        config.astDot = astDotArg.getValue();
        config.sourceFile = sourceFileArg.getValue();

        if (goodDefaultsArg.getValue()) {
            if (!staticAnalysisArg.isSet()) config.staticAnalysis = true;
            if (!optimizeArg.isSet()) config.optimize = true;
            if (!compileArg.isSet()) config.compile = true;
            if (!syntaxTreeDotArg.isSet()) config.syntaxTreeDot = false;
            if (!astDotArg.isSet()) config.astDot = false;
        }

        return config;

    } catch (TCLAP::ArgException &e) {
        using namespace caramel::colors;
        logger.fatal() << "Error during command line parsing: " << reset
                       << e.error() << " for arg " << e.argId();
        exit(1);
    }
}
