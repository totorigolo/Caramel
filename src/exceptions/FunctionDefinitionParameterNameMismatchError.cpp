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

#include "FunctionDefinitionParameterNameMismatchError.h"
#include "Common.h"



namespace caramel::exceptions {

using namespace colors;

    FunctionDefinitionParameterNameMismatchError::FunctionDefinitionParameterNameMismatchError(std::string const &message,
                                                 antlr4::ParserRuleContext *antlrContext,
                                                 std::string declaredName,
                                                 std::string definedName)
            : SemanticError(buildFunctionDefinitionParameterNameMismatchErrorMessage(message, declaredName, definedName), antlrContext){
    }

    void FunctionDefinitionParameterNameMismatchError::explain(utils::SourceFileUtil sourceFileUtil) const {
        //todo
        CARAMEL_UNUSED(sourceFileUtil);
        logger.fatal() << what();
    }

    std::string FunctionDefinitionParameterNameMismatchError::buildFunctionDefinitionParameterNameMismatchErrorMessage(const std::string &name,
                                                                                      std::string declaredName,
                                                                                      std::string definedName) {
        std::stringstream res;
        res << name << "'s parameter name " << definedName
            << " mismatches with the previously declared parameter " << declaredName << ".";
        return res.str();
    }

} // namespace caramel::exceptions
