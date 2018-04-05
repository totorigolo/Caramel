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

#include "PlusOperator.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/AdditionInstruction.h"
#include "../../../utils/Common.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/PushInstruction.h"
#include "../../../ir/instructions/PopInstruction.h"
#include "../../../ir/helpers/IROperatorHelper.h"

using namespace caramel::utils;

std::shared_ptr<caramel::ir::IR> caramel::ast::PlusOperator::getIR(
        std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
        std::shared_ptr<caramel::ast::Expression> const &leftExpression,
        std::shared_ptr<caramel::ast::Expression> const &rightExpression) {

    auto bb = currentBasicBlock;

    auto maxType = GET_MAX_TYPE(leftExpression, rightExpression);

    std::string left = GET_REGISTER(leftExpression);

    MOVE_TO(left, ir::IR::ACCUMULATOR_2, maxType);

    PUSH(ir::IR::ACCUMULATOR_2);

    std::string right = GET_REGISTER(rightExpression);

    MOVE_TO(right, ir::IR::ACCUMULATOR_1, maxType);

    POP(ir::IR::ACCUMULATOR_2);

    std::shared_ptr<ir::AdditionInstruction> instr = std::make_shared<ir::AdditionInstruction>(
            ir::IR::ACCUMULATOR_2,
            currentBasicBlock,
            maxType,
            ir::IR::ACCUMULATOR_1,
            ir::IR::ACCUMULATOR_2
    );

    return castTo<ir::IR::Ptr>(instr);
}

caramel::ast::StatementType caramel::ast::PlusOperator::getExpressionType() const {
    return StatementType::AdditiveExpression;
}

std::string caramel::ast::PlusOperator::getToken() const {
    return SYMBOL;
}

bool caramel::ast::PlusOperator::shouldReturnAnIR() const {
    return true;
}

bool caramel::ast::PlusOperator::shouldReturnABasicBlock() const {
    return false;
}
