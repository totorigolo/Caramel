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

#pragma once

#include <stdexcept>


namespace caramel::exceptions {

class UndefinedSymbolError : public SemanticError {

public:
    UndefinedSymbolError(std::string const &message,
                         antlr4::ParserRuleContext *antlrContext)
            : SemanticError(message),
              mAntlrContext{antlrContext} {}


    void explain(SourceFileUtil sourceFileUtil) const override {
        using namespace caramel::colors;

        // TODO: Create helper functions
        const int LEFT_MARGIN = 4;

        // Get shorter names for these
        auto const &start = mAntlrContext->getStart();
        auto const startLine = start->getLine();
        auto const startColumn = int(start->getCharPositionInLine());
        auto const &stop = mAntlrContext->getStop();
        auto const length = int(mAntlrContext->getText().length());

        // TODO: Handle multi-line statements
        if (start->getLine() != stop->getLine()) {
            logger.warning() << "Errors are buggy for multi-line statements.";
        }

        // Strip the left and right spaces
        std::string line(sourceFileUtil.getLine(startLine));
        size_t begin = line.find_first_not_of(' ');
        size_t end = line.find_last_not_of(' ') + 1;
        line = line.substr(begin, end - begin);

        std::stringstream posInfoSS;
        posInfoSS << startLine << ':' << startColumn;
        std::string posInfo(posInfoSS.str());
        auto posInfoLength = int(posInfo.length());

        // Print the error
        std::cerr << red << bold << "semantic error at " << posInfo << ": " << reset
                  << what() << std::endl
                  << posInfo << std::setfill(' ') << std::setw(LEFT_MARGIN) << ""
                  << line << std::endl
                  << std::setfill(' ') << std::setw(LEFT_MARGIN + posInfoLength + startColumn - int(begin)) << ""
                  << bold << red << std::setfill('~') << std::setw(length) << "" << reset
                  << std::endl;
    }

private:
    antlr4::ParserRuleContext *mAntlrContext;
};

} // namespace caramel::exception
