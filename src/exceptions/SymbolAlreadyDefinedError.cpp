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


#include "SymbolAlreadyDefinedError.h"
#include "../ast/symboltable/Symbol.h"


namespace caramel::exceptions {

using namespace colors;

SymbolAlreadyDefinedError::SymbolAlreadyDefinedError(std::string const &message,
                                                     std::shared_ptr<ast::Symbol> symbol,
                                                     antlr4::ParserRuleContext *antlrContext,
                                                     std::shared_ptr<ast::Definition> const &existingDefinition,
                                                     std::shared_ptr<ast::Declaration> const &faultyDeclaration)
        : SemanticError(buildAlreadyDefinedErrorMessage(message, symbol), antlrContext),
          mExistingDefinition{existingDefinition},
          mFaultyDeclaration{faultyDeclaration},
          mSymbol{symbol}{}

void SymbolAlreadyDefinedError::note() const {
    if (1) {
        //TODO: test if different type, return primary type (int32_t) instead of statement type (VariableDeclaration)
        std::cerr << bold << "Note: " << reset
                  << "different previous type: was " << mSymbol->getType()->getIdentifier()
                  << "and is now " << mFaultyDeclaration->getType()
                  << std::endl;
    }
}

std::string
SymbolAlreadyDefinedError::buildAlreadyDefinedErrorMessage(std::string const &variableName,
                                                           std::shared_ptr<ast::Symbol> symbol) {

    std::stringstream res;
    res << "Cannot use identifier: '" << variableName << "' because ";
    switch (symbol->getSymbolType()) {
        case ast::SymbolType::FunctionSymbol:
            res << "a function with the same name is already defined at line "
                << symbol->getDefinition()->getLine();
            break;
        case ast::SymbolType::VariableSymbol:
            res << "a variable with the same name is already defined at line "
                << symbol->getDefinition()->getLine();
            break;
        case ast::SymbolType::ArraySymbol:
            res << "an array with the same name is already defined at line "
                << symbol->getDefinition()->getLine();
            break;
        case ast::SymbolType::TypeSymbol:
            res << variableName << " is a reserved type identifier";
            break;
    }
    return res.str();
}

} // namespace caramel::exceptions
