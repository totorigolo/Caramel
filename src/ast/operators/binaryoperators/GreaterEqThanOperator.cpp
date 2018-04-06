/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
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

#include "GreaterEqThanOperator.h"
#include "../../../utils/Common.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/PushInstruction.h"
#include "../../../ir/instructions/PopInstruction.h"
#include "../../../ir/helpers/IROperatorHelper.h"
#include "../../../ir/instructions/FlagToRegInstruction.h"

using namespace caramel::utils;

std::shared_ptr<caramel::ir::IR> caramel::ast::GreaterEqThanOperator::getIR(
        std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
        std::shared_ptr<caramel::ast::Expression> const &leftExpression,
        std::shared_ptr<caramel::ast::Expression> const &rightExpression
) {

    auto maxType = GET_MAX_TYPE(leftExpression, rightExpression);

    std::string left = SAFE_ADD_INSTRUCTION(leftExpression, currentBasicBlock);

    MOVE_TO(left, ir::IR::ACCUMULATOR_2, maxType);

    PUSH(ir::IR::ACCUMULATOR_2);

    std::string right = SAFE_ADD_INSTRUCTION(rightExpression, currentBasicBlock);

    MOVE_TO(right, ir::IR::ACCUMULATOR_1, maxType);

    POP(ir::IR::ACCUMULATOR_2);

    std::shared_ptr<ir::FlagToRegInstruction> instr = std::make_shared<ir::FlagToRegInstruction>(
            ir::IR::ACCUMULATOR_2,
            currentBasicBlock,
            PrimaryType::max(leftExpression->getPrimaryType(), rightExpression->getPrimaryType()),
            ir::IR::ACCUMULATOR_2, // left value
            ir::IR::ACCUMULATOR_1, // right value (be careful to PUSH/POP)
            ir::FlagToRegType::GreaterOrEq
    );
    return castTo<ir::IR::Ptr>(instr);
}

caramel::ast::StatementType caramel::ast::GreaterEqThanOperator::getExpressionType() const {
    return StatementType::ComparativeExpression;
}

std::string caramel::ast::GreaterEqThanOperator::getToken() const {
    return SYMBOL;
}