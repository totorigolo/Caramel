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

#include "ParserErrorListener.h"
#include "Common.h"
#include "../../Console.h"


namespace caramel::listeners {

ParserErrorListener::ParserErrorListener(const std::string &sourceFile) : mSourceStream(sourceFile) {}

void ParserErrorListener::syntaxError(
        caramel_unused antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        caramel_unused const std::string &msg,
        caramel_unused std::exception_ptr e
) {
    if (line != mLastErrorLine) {
        if (mLastErrorLine == static_cast<size_t>(-1)) {
            mLastErrorLine = 0;
        }
        using namespace caramel::colors;
        std::cerr << "Error at line " << line << ':' << charPositionInLine << std::endl;
        std::string textLine = mSourceStream.getLine(line, mLastErrorLine, false);
        std::cerr << red << textLine << reset << std::endl;
        std::cerr.width(charPositionInLine + 2); // + 2 is for the "^ " length
        std::cerr << "^ ";
        std::cerr.width();
        std::cerr << "Unexpected \"" << offendingSymbol->getText() << "\"" << std::endl;
    }

    mLastErrorLine = line;
}

void ParserErrorListener::reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                          size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts,
                                          antlr4::atn::ATNConfigSet *configs) {
    CARAMEL_UNUSED(recognizer);
    CARAMEL_UNUSED(dfa);
    CARAMEL_UNUSED(startIndex);
    CARAMEL_UNUSED(stopIndex);
    CARAMEL_UNUSED(exact);
    CARAMEL_UNUSED(ambigAlts);
    CARAMEL_UNUSED(configs);

    // TODO: Implement reportAmbiguity()
}

void ParserErrorListener::reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa,
                                                      size_t startIndex, size_t stopIndex,
                                                      const antlrcpp::BitSet &conflictingAlts,
                                                      antlr4::atn::ATNConfigSet *configs) {
    CARAMEL_UNUSED(recognizer);
    CARAMEL_UNUSED(dfa);
    CARAMEL_UNUSED(startIndex);
    CARAMEL_UNUSED(stopIndex);
    CARAMEL_UNUSED(conflictingAlts);
    CARAMEL_UNUSED(configs);

    // TODO: Implement reportAttemptingFullContext()
}

void
ParserErrorListener::reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa,
                                              size_t startIndex,
                                              size_t stopIndex, size_t prediction,
                                              antlr4::atn::ATNConfigSet *configs) {
    CARAMEL_UNUSED(recognizer);
    CARAMEL_UNUSED(dfa);
    CARAMEL_UNUSED(startIndex);
    CARAMEL_UNUSED(stopIndex);
    CARAMEL_UNUSED(prediction);
    CARAMEL_UNUSED(configs);

    // TODO: Implement reportContextSensitivity()
}

} // namespace caramel::listeners
