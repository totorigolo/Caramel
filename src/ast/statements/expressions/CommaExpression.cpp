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

#include "CommaExpression.h"
#include "../../../ir/helpers/IROperatorHelper.h"
#include "../../../ir/instructions/EmptyInstruction.h"


namespace caramel::ast {

CommaExpression::CommaExpression(antlr4::Token *startToken, std::vector<Expression::Ptr> expressions)
        : Expression(startToken, StatementType::CommaExpression),
          mExpressions{std::move(expressions)} {
    if (mExpressions.empty()) {
        logger.fatal() << "Comma expression with 0 expressions!";
        exit(1);
    }
}

PrimaryType::Ptr CommaExpression::getPrimaryType() const {
    return mExpressions.back()->getPrimaryType();
}

bool CommaExpression::shouldReturnAnIR() const {
    return true;
}

std::shared_ptr<ir::IR> CommaExpression::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock) {

    std::string exprLocation;
    for (auto const &expression : mExpressions) {
        exprLocation = SAFE_ADD_INSTRUCTION(expression, currentBasicBlock);
    }

    return std::make_shared<ir::EmptyInstruction>(
            exprLocation,
            currentBasicBlock,
            mExpressions.back()->getPrimaryType()
    );
}

void CommaExpression::acceptAstDotVisit() {
    addNode(thisId(), "CommaExpression: " + std::to_string(mExpressions.size()) + " children");
    visitChildrenAstDot();
}

void CommaExpression::visitChildrenAstDot() {
    size_t i = 0;
    for (auto const& expression : mExpressions) {
        addEdge(thisId(), expression->thisId(), std::to_string(i++));
        expression->acceptAstDotVisit();
    }
}

} // namespace caramel::ast
