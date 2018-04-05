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

#include "FunctionNameAlreadyDeclaredError.h"
#include "../ast/symboltable/Symbol.h"


namespace caramel::exceptions {

using namespace colors;

FunctionNameAlreadyDeclaredError::FunctionNameAlreadyDeclaredError(antlr4::ParserRuleContext *antlrContext,
                                                                   std::string const &name,
                                                                   std::shared_ptr<ast::Symbol> symbol)
        : SemanticError(buildFunctionNameAlreadyDeclaredErrorMessage(name, symbol), antlrContext) {}

std::string
FunctionNameAlreadyDeclaredError::buildFunctionNameAlreadyDeclaredErrorMessage(std::string const &name,
                                                                               std::shared_ptr<ast::Symbol> symbol) {


    std::stringstream res;
    res << name + " previously declared as ";

    std::shared_ptr<ast::Symbol> previousDeclaration = symbol;
    ast::Symbol rawSymbol = *symbol;
    switch (previousDeclaration->getSymbolType()) {
        case ast::SymbolType::FunctionSymbol:
            res << "a function";
            break;
        case ast::SymbolType::VariableSymbol:
            res << "a variable";
            break;
        case ast::SymbolType::TypeSymbol:
            res << "a reserved type identifier";
            break;
        case ast::SymbolType::ArraySymbol:
            res << "an array";
            break;
    }
    res << ", so it's impossible to redeclare it as a function.";

    return res.str();
}
} // namespace caramel::exceptions
