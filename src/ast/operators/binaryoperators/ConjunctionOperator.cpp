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

#include "ConjunctionOperator.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/LDConstInstruction.h"
#include "../../../ir/instructions/EmptyInstruction.h"
#include "../../../ir/instructions/NopInstruction.h"
#include "../../../utils/Common.h"

using namespace caramel::ast;
using namespace caramel::utils;
using namespace caramel;

int ConjunctionOperator::nb = 0;

ir::GetBasicBlockReturn ConjunctionOperator::getBasicBlock(ir::CFG *controlFlow,
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

    logger.warning() << "ConjunctionOperator[startBlock - getBBmain] Does it? #" << startBlock->getId();
    startBlock->setExitWhenTrue(trueBlock);
    startBlock->setExitWhenFalse(falseBlock);

    std::string returnName = Statement::createVarName();
    PrimaryType::Ptr Int64Type = std::make_shared<Int64_t>();

    logger.warning() << "ConjunctionOperator[trueBlock - getBBmain] Does it? #" << trueBlock->getId();
    trueBlock->setExitWhenTrue(endBlock);
    trueBlock->addInstruction(std::make_shared<ir::LDConstInstruction>(
            trueBlock,
            Int64Type,
            returnName,
            "1"
    ));

    logger.warning() << "ConjunctionOperator[falseBlock - getBBmain] Does it? #" << falseBlock->getId();
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
        auto ir = leftExpression->getIR(startBlock);
        startBlock->addInstruction(ir);
        lastLeftBlock = startBlock;
    } else {
        auto leftBlockChain = leftExpression->getBasicBlock(controlFlow);
        leftBlockChain.end->setExitWhenTrue(startBlock->getNextWhenTrue());
        leftBlockChain.end->setExitWhenFalse(startBlock->getNextWhenFalse());

        startBlock = leftBlockChain.begin;
        lastLeftBlock = leftBlockChain.end;

//        logger.warning() << "ConjunctionOperator[startBlock - leftBB] Does it? #" << startBlock->getId();
//        startBlock->setExitWhenTrue(leftBlockChain.begin);
//        lastLeftBlock = leftBlockChain.end;
//
//        logger.warning() << "ConjunctionOperator[lastLeftBlock - leftBB] Does it? #" << lastLeftBlock->getId();
//
//        lastLeftBlock->setExitWhenFalse(falseBlock);
    }

    if (rightExpression->shouldReturnAnIR()) {
        ir::BasicBlock::Ptr midBlock = controlFlow->generateBasicBlock(
                ir::BasicBlock::getNextNumberName() + "_" + std::to_string(currentNb) + "_and_mid");
        auto ir = rightExpression->getIR(midBlock);
        midBlock->addInstruction(ir);

        logger.warning() << "ConjunctionOperator[midBlock] Does it? #" << midBlock->getId();
        midBlock->setExitWhenTrue(trueBlock);
        midBlock->setExitWhenFalse(falseBlock);

        logger.warning() << "ConjunctionOperator[lastLeftBlock - rightIR] Does it? #" << lastLeftBlock->getId();
        lastLeftBlock->setExitWhenTrue(midBlock);
    } else {
        auto rightBlockChain = rightExpression->getBasicBlock(controlFlow);

        logger.warning() << "ConjunctionOperator[lastLeftBlock - rightBB] Does it? #" << lastLeftBlock->getId();
        lastLeftBlock->setExitWhenTrue(rightBlockChain.begin);


        logger.warning() << "ConjunctionOperator[rightBlockChain] Does it? #" << rightBlockChain.end->getId();
        rightBlockChain.end->setExitWhenTrue(trueBlock);
        rightBlockChain.end->setExitWhenFalse(falseBlock);
    }

    return {startBlock, constEnd};

}

std::shared_ptr<ir::IR> ConjunctionOperator::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock,
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

    logger.warning() << "ConjunctionOperator[getIR] Does it? #" << currentBasicBlock->getId();
    currentBasicBlock->setExitWhenTrue(start);
    currentBasicBlock->setExitWhenFalse(nullptr);
    currentBasicBlock = end;

    return castTo<ir::IR::Ptr>(std::make_shared<ir::EmptyInstruction>(
            currentBasicBlock->getInstructions().back()->getReturnName(),
            currentBasicBlock,
            currentBasicBlock->getInstructions().back()->getType()
    ));
}

StatementType ConjunctionOperator::getExpressionType() const {
    return StatementType::ConjunctionExpression;
}

std::string ConjunctionOperator::getToken() const {
    return SYMBOL;
}

bool ConjunctionOperator::shouldReturnAnIR() const {
    return false;
}

bool ConjunctionOperator::shouldReturnABasicBlock() const {
    return true;
}
