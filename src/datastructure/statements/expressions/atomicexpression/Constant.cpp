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

#include "Constant.h"
#include "../../../../ir/IR.h"

namespace caramel::ast {

Constant::Constant(long long mValue, antlr4::Token *startToken, StatementType type)
        : AtomicExpression(startToken, type),
        mValue(mValue) {
}

long long Constant::getValue() {
    return mValue;
}

void Constant::acceptAstDotVisit() {
    addNode(thisId(), "Constant: " + std::to_string(mValue));
}

void Constant::visitChildrenAstDot() {}

bool Constant::shouldReturnAnIR() const {
    return true;
}

std::shared_ptr<ir::IR> Constant::getIR(std::shared_ptr<caramel::ir::BasicBlock> const &currentBasicBlock) {

    std::string tempVar = createVarName();
    std::string constValue = std::to_string(getValue());
    std::vector<std::string> parameters;
    parameters.push_back(tempVar);
    parameters.push_back(constValue);
    // Todo: check if Int64_t for constant is good or not
    return std::make_shared<ir::IR>(tempVar, currentBasicBlock, ir::Operation::ldconst, Int64_t::Create(), parameters);
}


} // namespace caramel::ast
