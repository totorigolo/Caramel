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

#include "SemanticError.h"
#include "FunctionDefinitionParameterTypeMismatchError.h"
#include "Common.h"
#include "../ast/symboltable/PrimaryType.h"

#include <ParserRuleContext.h>


namespace caramel::exceptions {

using namespace ast;
using namespace colors;

FunctionDefinitionParameterTypeMismatchError::FunctionDefinitionParameterTypeMismatchError(antlr4::ParserRuleContext *antlrContext,
                                                 std::shared_ptr<PrimaryType> declaredType,
                                                 std::shared_ptr<PrimaryType> definedType)
            : SemanticError(buildFunctionDefinitionParameterTypeMismatchErrorMessage(declaredType, definedType),
                            antlrContext) {
    }

    //explain différent du commun dans Semantic Error
    /*void FunctionDefinitionParameterTypeMismatchError::explain(utils::SourceFileUtil sourceFileUtil) const {
        //todo
        CARAMEL_UNUSED(sourceFileUtil);
        logger.fatal() << what();
    }*/

    std::string FunctionDefinitionParameterTypeMismatchError::buildFunctionDefinitionParameterTypeMismatchErrorMessage(
            std::shared_ptr<PrimaryType> declaredType,
            std::shared_ptr<PrimaryType> declaredName) {
        std::stringstream res;
        res << "The function: "
            << declaredType->getIdentifier()
            << " was previously declared with a parameter of type "
            << declaredName
            << " .\n"
            << "Actual parameter type is "
            << declaredName->getIdentifier()
            << ".";
        return res.str();
    }

} // namespace caramel::exceptions
