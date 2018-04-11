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

#include "DisjunctionOperator.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/LDConstInstruction.h"
#include "../../../ir/instructions/EmptyInstruction.h"
#include "../../../ir/instructions/NopInstruction.h"
#include "../../../utils/Common.h"
#include "../../../ir/helpers/IROperatorHelper.h"

using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel;

int DisjunctionOperator::nb = 0;

ir::GetBasicBlockReturn DisjunctionOperator::getBasicBlock(ir::CFG *controlFlow,
                                                           std::shared_ptr<Expression> const &leftExpression,
                                                           std::shared_ptr<Expression> const &rightExpression) {
    nb++;
    int currentNb = nb;
    ir::BasicBlock::Ptr startBlock = controlFlow->generateBasicBlock(
            ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_start");
    ir::BasicBlock::Ptr trueBlock  = controlFlow->generateBasicBlock(
            ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_true");
    ir::BasicBlock::Ptr falseBlock  = controlFlow->generateBasicBlock(
            ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_false");
    ir::BasicBlock::Ptr endBlock = controlFlow->generateBasicBlock(
            ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_end");

    ir::BasicBlock::Ptr constEnd = endBlock;

    startBlock->setExitWhenTrue(trueBlock);
    startBlock->setExitWhenFalse(falseBlock);

    std::string returnName = Statement::createVarName();
    PrimaryType::Ptr Int64Type = std::make_shared<Int64_t>();

    trueBlock->setExitWhenTrue(endBlock);
    trueBlock->addInstruction(std::make_shared<ir::LDConstInstruction>(
            trueBlock,
            Int64Type,
            returnName,
            "1"
    ));

    falseBlock->setExitWhenTrue(endBlock);
    falseBlock->addInstruction(std::make_shared<ir::LDConstInstruction>(
            falseBlock,
            Int64Type,
            returnName,
            "0"
    ));

    endBlock->addInstruction(std::make_shared<ir::EmptyInstruction>(
            returnName,
            endBlock,
            Int64Type
    ));

    ir::BasicBlock::Ptr lastLeftBlock;

    if (leftExpression->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(leftExpression, startBlock);
        lastLeftBlock = startBlock;
    } else {
        auto leftBlockChain = leftExpression->getBasicBlock(controlFlow);
        leftBlockChain.end->setExitWhenTrue(startBlock->getNextWhenTrue());
        leftBlockChain.end->setExitWhenFalse(startBlock->getNextWhenFalse());

        startBlock = leftBlockChain.begin;
        lastLeftBlock = leftBlockChain.end;
    }

    if (rightExpression->shouldReturnAnIR()) {
        ir::BasicBlock::Ptr midBlock = controlFlow->generateBasicBlock(
                ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_mid");
        SAFE_ADD_INSTRUCTION(rightExpression, midBlock);

        midBlock->setExitWhenTrue(trueBlock);
        midBlock->setExitWhenFalse(falseBlock);

        lastLeftBlock->setExitWhenFalse(midBlock); //< this is the only difference with conjunction !
    } else {
        auto rightBlockChain = rightExpression->getBasicBlock(controlFlow);

        lastLeftBlock->setExitWhenFalse(rightBlockChain.begin); //< this is the only difference with conjunction !

        rightBlockChain.end->setExitWhenTrue(trueBlock);
        rightBlockChain.end->setExitWhenFalse(falseBlock);
    }

    return {startBlock, constEnd};
}

std::shared_ptr<ir::IR> DisjunctionOperator::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
                                                   std::shared_ptr<Expression> const &leftExpression,
                                                   std::shared_ptr<Expression> const &rightExpression) {
    auto [start, end] = getBasicBlock(currentBasicBlock->getCFG(), leftExpression, rightExpression);

    auto nextTrue  = currentBasicBlock->getNextWhenTrue();
    auto nextFalse = currentBasicBlock->getNextWhenFalse();

    if (nextTrue) {
        end->setExitWhenTrue(nextTrue);
    }
    if (nextFalse) {
        end->setExitWhenFalse(nextFalse);
    }

    currentBasicBlock->setExitWhenTrue(start);
    currentBasicBlock->setExitWhenFalse(nullptr);
    currentBasicBlock = end;

    return castTo<ir::IR::Ptr>(std::make_shared<ir::EmptyInstruction>(
            currentBasicBlock->getInstructions().back()->getReturnName(),
            currentBasicBlock,
            currentBasicBlock->getInstructions().back()->getType()
    ));
}

StatementType DisjunctionOperator::getExpressionType() const {
    return StatementType::ConjunctionExpression;
}

std::string DisjunctionOperator::getToken() const {
    return SYMBOL;
}

bool DisjunctionOperator::shouldReturnAnIR() const {
    return false;
}

bool DisjunctionOperator::shouldReturnABasicBlock() const {
    return true;
}
