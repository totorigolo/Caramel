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
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/AdditionInstruction.h"
#include "../../../utils/Common.h"
#include "../../../ir/instructions/GTEInstruction.h"

using namespace caramel::utils;

std::shared_ptr<caramel::ir::IR> caramel::ast::GreaterEqThanOperator::getIR(
        std::shared_ptr<caramel::ir::BasicBlock> const &currentBasicBlock,
        std::shared_ptr<caramel::ast::Expression> const &leftExpression,
        std::shared_ptr<caramel::ast::Expression> const &rightExpression
) {

    std::string var1 = currentBasicBlock->addInstruction(leftExpression->getIR(currentBasicBlock));
    std::string var2 = currentBasicBlock->addInstruction(rightExpression->getIR(currentBasicBlock));
    std::string tmp = Statement::createVarName();

    std::shared_ptr<ir::GTEInstruction> instr = std::make_shared<ir::GTEInstruction>(
            tmp,
            currentBasicBlock,
            PrimaryType::max(leftExpression->getPrimaryType(), rightExpression->getPrimaryType())
    );
    return castTo<ir::IR::Ptr>(instr);
}

caramel::ast::StatementType caramel::ast::GreaterEqThanOperator::getExpressionType() const {
    return StatementType::ComparativeExpression;
}

std::string caramel::ast::GreaterEqThanOperator::getToken() const {
    return SYMBOL;
}