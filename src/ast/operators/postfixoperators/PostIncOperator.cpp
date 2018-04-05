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

#include "PostIncOperator.h"
#include "../../../ir/instructions/AdditionInstruction.h"
#include "../../statements/expressions/atomicexpression/LValue.h"
#include "../../../utils/Common.h"
#include "../../../ir/helpers/IROperatorHelper.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/LDConstInstruction.h"
#include "../../../ir/instructions/SubtractionInstruction.h"

using namespace caramel::utils;

std::shared_ptr<caramel::ir::IR> caramel::ast::PostIncOperator::buildIR(
        std::shared_ptr<caramel::ir::BasicBlock> &currentBasicBlock,
        std::shared_ptr<caramel::ast::Expression> const &expression
) {


    LValue::Ptr lvalue = castTo<LValue::Ptr>(expression);

    ir::IR::Ptr copy = std::make_shared<ir::LDConstInstruction>(
            currentBasicBlock,
            lvalue->getPrimaryType(),
            Statement::createVarName(),
            lvalue->getSymbol()->getName()
    );
    std::string tmpName = currentBasicBlock->addInstruction(copy);

    ir::IR::Ptr addition = std::make_shared<ir::AdditionInstruction>(
            ir::IR::ACCUMULATOR_1,
            currentBasicBlock,
            lvalue->getPrimaryType(),
            "1",
            tmpName
    );
    std::string tmpName2 = currentBasicBlock->addInstruction(addition);

    ir::IR::Ptr copyBack = std::make_shared<ir::LDConstInstruction>(
            currentBasicBlock,
            lvalue->getPrimaryType(),
            lvalue->getSymbol()->getName(),
            tmpName2
    );
    currentBasicBlock->addInstruction(copyBack);

    ir::IR::Ptr substraction = std::make_shared<ir::SubtractionInstruction>(
            ir::IR::ACCUMULATOR_1,
            currentBasicBlock,
            lvalue->getPrimaryType(),
            "1",
            tmpName
    );
    return substraction;

}

caramel::ast::StatementType caramel::ast::PostIncOperator::getExpressionType() const {
    return StatementType::UnaryAdditiveExpression;
}

std::string caramel::ast::PostIncOperator::getToken() const {
    return SYMBOL;
}
