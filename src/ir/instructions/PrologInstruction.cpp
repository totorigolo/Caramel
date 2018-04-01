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

#include "PrologInstruction.h"
#include "../IRVisitor.h"

namespace caramel::ir {

PrologInstruction::PrologInstruction(
        std::shared_ptr<BasicBlock> const &parentBlock,
        std::string functionName,
        std::vector<ast::FunctionParameterSignature> parameters
) : IR("", parentBlock, Operation::prolog, ast::Void_t::Create()),
    mFunctionName{std::move(functionName)},
    mParameters{std::move(parameters)} {}

void PrologInstruction::accept(std::shared_ptr<IRVisitor> const &visitor, std::ostream &os) {
    visitor->visitProlog(this, os);
}

std::string const &PrologInstruction::getFunctionName() const {
    return mFunctionName;
}

std::vector<ast::FunctionParameterSignature> const &PrologInstruction::getParameters() const {
    return mParameters;
}

} // namespace caramel::ir