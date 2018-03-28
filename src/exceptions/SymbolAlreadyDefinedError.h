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

#include "../Console.h"

#include <stdexcept>


namespace caramel::exceptions {

class SymbolAlreadyDefinedError : public SemanticError {
public:
    SymbolAlreadyDefinedError(std::string const &message,
                              Symbol::Ptr symbol,
                              antlr4::ParserRuleContext *antlrContext,
                              caramel::ast::Definition::Ptr const &existingDefinition,
                              caramel::ast::Declaration::Ptr const &faultyDeclaration)
            : SemanticError(buildAlreadyDefinedErrorMessage(message, symbol)),
              mAntlrContext{antlrContext},
              mExistingDefinition{existingDefinition},
              mFaultyDeclaration{faultyDeclaration} {
    }

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
        if (1) {
            //TODO: test if different type, return primary type instead of statement type
            std::cerr << bold << "Note: " << reset
                      << "different previous type, was " << mExistingDefinition->getType() << " now "
                      << mFaultyDeclaration->getType()
                      << std::endl;
        }
    }

    std::string
    buildAlreadyDefinedErrorMessage(std::string const &variableName, Symbol::Ptr symbol) {

        std::stringstream res;
        res << "Cannot use identifier: " << variableName << " because ";
        Symbol::Ptr previousDeclaration = symbol;
        switch (previousDeclaration->getSymbolType()) {
            case SymbolType::FunctionSymbol:
                res << "a function with the same name is already declared at line "
                    << previousDeclaration->getDefinition()->getLine();
                break;
            case SymbolType::VariableSymbol:
                res << "a variable with the same name is already declared at line "
                    << previousDeclaration->getDefinition()->getLine();
                break;
            case SymbolType::TypeSymbol:
                res << variableName << " is a reserved type identifier";
                break;
        }
        return res.str();

    }

private:
    antlr4::ParserRuleContext *mAntlrContext;
    caramel::ast::Definition::Ptr mExistingDefinition;
    caramel::ast::Declaration::Ptr mFaultyDeclaration;
};

} // namespace caramel::exceptions
