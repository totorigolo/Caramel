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

#include "ArrayAccess.h"
#include "../../../../ir/instructions/EmptyInstruction.h"
#include "../../../../ir/instructions/CopyInstruction.h"
#include "../../../../ir/instructions/NopInstruction.h"
#include "../../../../ir/instructions/ArrayAccessCopyInstruction.h"
#include "../../../../ir/instructions/PopInstruction.h"
#include "../../../../ir/helpers/IROperatorHelper.h"

namespace caramel::ast {

ArrayAccess::ArrayAccess(
        caramel::ast::Expression::Ptr index,
        antlr4::Token *startToken
) : LValue(startToken, StatementType::ArrayAccess), mIndex{std::move(index)} {}

Symbol::Ptr ArrayAccess::getSymbol() const {
    return mSymbol;
}

void ArrayAccess::setSymbol(ArraySymbol::Ptr symbol) {
    mSymbol = std::move(symbol);
}

PrimaryType::Ptr ArrayAccess::getPrimaryType() const {
    return mSymbol->getType();
}

void ArrayAccess::acceptAstDotVisit() {
    addNode(thisId(), "Array Access: " + mSymbol->getName());
}

void ArrayAccess::visitChildrenAstDot() {
    addEdge(thisId(), mIndex->thisId());
    mIndex->acceptAstDotVisit();
}

bool ArrayAccess::shouldReturnAnIR() const {
    return true;
}

std::shared_ptr<ir::IR> ArrayAccess::getIR(std::shared_ptr<caramel::ir::BasicBlock> &currentBasicBlock) {

//    movl -4(%rbp), %eax
    std::string indexSrc = SAFE_ADD_INSTRUCTION(mIndex, currentBasicBlock); // currentBasicBlock->addInstruction(mIndex->getIR(currentBasicBlock));
    auto prevSrc = currentBasicBlock->addInstruction(std::make_shared<ir::CopyInstruction>(
            currentBasicBlock, mSymbol->getType(), createVarName(), indexSrc
    ));

//    cltq
//    movl -16(%rbp,%rax,4), %eax
    if (isUsedInLeft()) {
        currentBasicBlock->addInstruction(std::make_shared<ir::PopInstruction>(
                currentBasicBlock, mSymbol->getType(), ir::IR::ACCUMULATOR
        ));
        return std::make_shared<ir::ArrayAccessCopyInstruction>(
                currentBasicBlock, mSymbol->getType(), ir::IR::ACCUMULATOR, prevSrc, mSymbol->getName(), isUsedInLeft()
        );
    } else {
        return std::make_shared<ir::ArrayAccessCopyInstruction>(
                currentBasicBlock, mSymbol->getType(), createVarName(), prevSrc, mSymbol->getName(), isUsedInLeft()
        );
    }
}

} // namespace caramel::ast
