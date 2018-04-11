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
#include "../../../utils/Common.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/helpers/IROperatorHelper.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/EmptyInstruction.h"
#include "../../../ir/instructions/AdditionInstruction.h"
#include "../../statements/expressions/atomicexpression/LValue.h"

using namespace caramel::utils;

std::shared_ptr<caramel::ir::IR> caramel::ast::PostIncOperator::buildIR(
        std::shared_ptr<caramel::ir::BasicBlock> &currentBasicBlock,
        std::shared_ptr<caramel::ast::Expression> const &expression
) {
    auto lvalue = castTo<LValue::Ptr>(expression);
    std::string oldValue = Statement::createVarName();
    currentBasicBlock->addInstruction(std::make_shared<ir::CopyInstruction>(
            currentBasicBlock,
            expression->getPrimaryType(),
            oldValue,
            lvalue->getSymbol()->getName()
    ));

    std::string tmpName = Statement::createVarName();
    std::string lvalueRegister = SAFE_ADD_INSTRUCTION(expression, currentBasicBlock);
    currentBasicBlock->addInstruction(castTo<ir::IR::Ptr>(std::make_shared<ir::AdditionInstruction>(
            tmpName,
            currentBasicBlock,
            expression->getPrimaryType(),
            lvalueRegister,
            "1"
    )));

    currentBasicBlock->addInstruction(std::make_shared<ir::CopyInstruction>(
            currentBasicBlock,
            expression->getPrimaryType(),
            lvalue->getSymbol()->getName(),
            tmpName
    ));

    return castTo<ir::IR::Ptr>(std::make_shared<ir::EmptyInstruction>(
            oldValue,
            currentBasicBlock,
            expression->getPrimaryType()
    ));
}

caramel::ast::StatementType caramel::ast::PostIncOperator::getExpressionType() const {
    return StatementType::UnaryAdditiveExpression;
}

std::string caramel::ast::PostIncOperator::getToken() const {
    return SYMBOL;
}
