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

#include "FunctionDefinitionParameterTypeMismatchError.h"
#include "../ast/symboltable/PrimaryType.h"


namespace caramel::exceptions {

using namespace ast;

FunctionDefinitionParameterTypeMismatchError::FunctionDefinitionParameterTypeMismatchError(
        antlr4::ParserRuleContext *antlrContext,
        std::string const &name,
        std::shared_ptr<PrimaryType> declaredType,
        std::shared_ptr<PrimaryType> definedType)
        : SemanticError(buildFunctionDefinitionParameterTypeMismatchErrorMessage(name, declaredType, definedType),
                        antlrContext) {
}

void FunctionDefinitionParameterTypeMismatchError::note() const {
}

std::string FunctionDefinitionParameterTypeMismatchError::buildFunctionDefinitionParameterTypeMismatchErrorMessage(
        std::string const &name,
        std::shared_ptr<PrimaryType> declaredType,
        std::shared_ptr<PrimaryType> definedType) {
    std::stringstream res;
    res << "The function: '"
        << name
        << "' was previously declared with a parameter of type "
        << declaredType->getIdentifier()
        << " .\n"
        << "Parameter type found is "
        << definedType->getIdentifier()
        << ".";
    return res.str();
}

} // namespace caramel::exceptions
