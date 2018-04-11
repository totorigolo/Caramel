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

#include "ReturnStatement.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/ReturnInstruction.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

ReturnStatement::ReturnStatement(
        antlr4::Token *startToken
) : ReturnStatement(nullptr, startToken) {}

ReturnStatement::ReturnStatement(
        std::shared_ptr<Expression> expression,
        antlr4::Token *startToken
) : Jump(startToken, StatementType::ReturnStatement), mExpression{std::move(expression)} {}

void ReturnStatement::acceptAstDotVisit() {
    addNode(thisId(), "ReturnStatement");
    visitChildrenAstDot();
}

void ReturnStatement::visitChildrenAstDot() {
    if (mExpression) {
        addEdge(thisId(), mExpression->thisId());
        mExpression->acceptAstDotVisit();
    }
}

bool ReturnStatement::shouldReturnAnIR() const {
    return true;
}

std::shared_ptr<ir::IR> ReturnStatement::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock) {
    if (mExpression) {
        std::string value = SAFE_ADD_INSTRUCTION(mExpression, currentBasicBlock); // currentBasicBlock->addInstruction(mExpression->getIR(currentBasicBlock));
        return std::make_shared<ir::ReturnInstruction>(
                currentBasicBlock, mExpression->getPrimaryType(), value);
    } else {
        return std::make_shared<ir::ReturnInstruction>(currentBasicBlock);
    }
}


} // namespace caramel::ast
