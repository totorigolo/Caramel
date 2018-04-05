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

#include "FunctionCallInstruction.h"
#include "../IRVisitor.h"


namespace caramel::ir {

FunctionCallInstruction::FunctionCallInstruction(
        std::string functionName,
        std::shared_ptr<BasicBlock> const &parentBlock,
        ast::PrimaryType::Ptr const &returnType
) : IR(IR::ACCUMULATOR, Operation::call, parentBlock, returnType),
    mFunctionName{std::move(functionName)} {}

std::string FunctionCallInstruction::getFunctionName() const {
    return mFunctionName;
}

void FunctionCallInstruction::accept(std::shared_ptr<IRVisitor> const &visitor, std::ostream &os) {
    visitor->visitFunctionCall(this, os);
}

} // namespace caramel::ir
