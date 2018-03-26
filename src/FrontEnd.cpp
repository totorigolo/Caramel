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

#include "FrontEnd.h"

#include "Logger.h"
#include "listeners/errorlistener/ParserErrorListener.h"
#include "exceptions/SemanticError.h"


namespace caramel {

ast::Context::Ptr frontEnd(Config const &config) {
    CARAMEL_UNUSED(config);

    // Read the source file
    ifstream sourceFile(config.sourceFile);

    // All the Antlr stuff
    ANTLRInputStream input(sourceFile);
    CaramelLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CaramelParser parser(&tokens);

    ParserErrorListener errorListener(config.sourceFile);
    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);

    // Generate the dot of the tree if asked
    if (config.syntaxTreeDot) {
        Listeners::DotExportListener dotExport(&parser);
        tree::ParseTreeWalker::DEFAULT.walk(&dotExport, parser.r());

        // Write the dot file to disc
        ofstream out;
        out.open("out.dot");
        out << dotExport.getDotFile();
        out.close();
    }

    // Create the visitor which will generate the AST
    caramel::visitors::AbstractSyntaxTreeVisitor abstractSyntaxTreeVisitor(config.sourceFile);
    try {
        auto visitorResult = abstractSyntaxTreeVisitor.visit(parser.r());
        if (!visitorResult.is<Context::Ptr>()) {
            using namespace caramel::colors;
            logger.fatal() << "The visitor returned a bad root.";
            exit(1);
        }

        // Return the AST root
        return visitorResult.as<Context::Ptr>();

    } catch (caramel::exceptions::SemanticError &semanticError) {
        semanticError.explain(SourceFileUtil(config.sourceFile));
        exit(1);
    }
}

} // namespace Caramel
