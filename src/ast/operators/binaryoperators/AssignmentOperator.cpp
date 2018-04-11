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
#include "../../../ir/instructions/ArrayAccessInstruction.h"
#include "../../statements/expressions/atomicexpression/LValue.h"
#include "../../statements/expressions/atomicexpression/Constant.h"
#include "../../statements/expressions/atomicexpression/ArrayAccess.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

using namespace utils;

std::shared_ptr<ir::IR> AssignmentOperator::getIR(
        std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
        Expression::Ptr const &leftExpression,
        Expression::Ptr const &rightExpression) {

    auto maxType = GET_MAX_TYPE(leftExpression, rightExpression);

    auto lvalueExpr = castTo<LValue::Ptr>(leftExpression);
    lvalueExpr->setIsUsedAsLeftValue(true);

    if (lvalueExpr->getType() == StatementType::ArrayAccess) {

        auto arrayAccess = castTo<ArrayAccess::Ptr>(lvalueExpr);
        Expression::Ptr indexExpr = arrayAccess->getIndex();
        std::string arrayName = arrayAccess->getArrayName();

        std::string rvalue = SAFE_ADD_INSTRUCTION(rightExpression, currentBasicBlock);
        std::string index = SAFE_ADD_INSTRUCTION(indexExpr, currentBasicBlock);

        return castTo<ir::IR::Ptr>(std::make_shared<ir::ArrayAccessInstruction>(
                currentBasicBlock,
                maxType,
                Statement::createVarName(),
                index, indexExpr->getPrimaryType(),
                arrayName,
                true, rvalue
        ));

    } else {
        std::string lvalue = SAFE_ADD_INSTRUCTION(lvalueExpr, currentBasicBlock);

        std::string rvalue;
        if (castTo<LValue::Ptr>(rightExpression)) {
            LValue::Ptr rightExpressionAsLValue = castTo<LValue::Ptr>(rightExpression);
            rvalue = currentBasicBlock->addInstruction(
                    std::make_shared<ir::AdditionInstruction>(
                            Statement::createVarName(),
                            currentBasicBlock,
                            maxType,
                            rightExpressionAsLValue->getSymbol()->getName(),
                            "0"
                    ) // TODO: Fix this workaround
            );
        } else {
            rvalue = SAFE_ADD_INSTRUCTION(rightExpression, currentBasicBlock);
        }

        return castTo<ir::IR::Ptr>(std::make_shared<ir::CopyInstruction>(
                currentBasicBlock,
                maxType,
                lvalue,
                rvalue
        ));
    }
}

StatementType AssignmentOperator::getExpressionType() const {
    return StatementType::AdditiveExpression;
}

std::string AssignmentOperator::getToken() const {
    return SYMBOL;
}

} // namespace caramel::ast
