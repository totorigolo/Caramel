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

#include "AssignmentOperator.h"
#include "../../../utils/Common.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/AdditionInstruction.h"
#include "../../statements/expressions/atomicexpression/LValue.h"
#include "../../../ir/instructions/PushInstruction.h"


namespace caramel::ast {

using namespace utils;

AssignmentOperator::AssignmentOperator(LValue::Ptr lvalue)
        : mLValue{std::move(lvalue)} {}

std::shared_ptr<ir::IR> AssignmentOperator::getIR(
        std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
        Expression::Ptr const &leftExpression,
        Expression::Ptr const &rightExpression) {

    mLValue->setIsUsedInLeft(true);

    if (mLValue->getType() == StatementType::ArrayAccess) {

        std::string rvalue = currentBasicBlock->addInstruction(rightExpression->getIR(currentBasicBlock));
        currentBasicBlock->addInstruction(std::make_shared<ir::PushInstruction>(
                currentBasicBlock, rightExpression->getPrimaryType(), rvalue
        ));

        std::string lvalue = currentBasicBlock->addInstruction(mLValue->getIR(currentBasicBlock));

        std::shared_ptr<ir::CopyInstruction> instr = std::make_shared<ir::CopyInstruction>(
                currentBasicBlock,
                PrimaryType::max(leftExpression->getPrimaryType(), rightExpression->getPrimaryType()),
                lvalue,
                rvalue
        );
        return castTo<ir::IR::Ptr>(instr);

    } else {
        std::string lvalue = currentBasicBlock->addInstruction(mLValue->getIR(currentBasicBlock));
        std::string rvalue = currentBasicBlock->addInstruction(rightExpression->getIR(currentBasicBlock));

        std::shared_ptr<ir::CopyInstruction> instr = std::make_shared<ir::CopyInstruction>(
                currentBasicBlock,
                PrimaryType::max(leftExpression->getPrimaryType(), rightExpression->getPrimaryType()),
                lvalue,
                rvalue
        );
        return castTo<ir::IR::Ptr>(instr);
    }
}

StatementType AssignmentOperator::getExpressionType() const {
    return StatementType::AdditiveExpression;
}

std::string AssignmentOperator::getToken() const {
    return SYMBOL;
}

} // namespace caramel::ast
