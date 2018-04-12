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

#include "IfBlock.h"
#include "../../../ir/CFG.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/IR.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

IfBlock::IfBlock(
        std::shared_ptr<caramel::ast::Expression> const &condition,
        std::vector<std::shared_ptr<caramel::ast::Statement>> const &thenBlock,
        std::vector<std::shared_ptr<caramel::ast::Statement>> const &elseBlock,
        antlr4::Token *startToken
) : ControlBlock(startToken, StatementType::IfBlock),
    mCondition{condition},
    mThenBlock{thenBlock},
    mElseBlock{elseBlock} {}

void IfBlock::acceptAstDotVisit() {
    addNode(thisId(), "If");
    visitChildrenAstDot();
}

void IfBlock::visitChildrenAstDot() {
    addEdge(thisId(), mCondition->thisId(), "condition");
    mCondition->acceptAstDotVisit();

    addNode(thisId() + 1, "Then block");
    addEdge(thisId(), thisId() + 1);
    for (const auto &thenStatement : mThenBlock) {
        addEdge(thisId() + 1, thenStatement->thisId());
        thenStatement->acceptAstDotVisit();
    }

    if (!mElseBlock.empty()) {
        addNode(thisId() + 2, "Else block");
        addEdge(thisId(), thisId() + 2);
        for (const auto &elseStatement : mElseBlock) {
            addEdge(thisId() + 2, elseStatement->thisId());
            elseStatement->acceptAstDotVisit();
        }
    }
}

ir::GetBasicBlockReturn IfBlock::getBasicBlock(
        ir::CFG *controlFlow
) {
    ir::BasicBlock::Ptr bbCond = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_cond");
    ir::BasicBlock::Ptr bbThen = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_then");
    ir::BasicBlock::Ptr bbElse = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_else");
    ir::BasicBlock::Ptr bbEnd = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_end");

    bbCond->setExitWhenTrue(bbThen);
    bbThen->setExitWhenTrue(bbEnd);

    // COND BB
    if (mCondition->shouldReturnAnIR()) {
        // TODO: Grab the returnName here? (I think not)
        SAFE_ADD_INSTRUCTION(mCondition, bbCond); // bbCond->addInstruction(mCondition->getIR(bbCond));
    } else if (mCondition->shouldReturnABasicBlock()) {

        auto [cond_begin, cond_end] = mCondition->getBasicBlock(controlFlow);

        bbCond = cond_begin;
        cond_end->setExitWhenTrue(bbThen);
        cond_end->setExitWhenFalse(bbElse);
    }

    // THEN BB
    for (auto const &statement : mThenBlock) {
        if (statement->shouldReturnAnIR()) {
            SAFE_ADD_INSTRUCTION(statement, bbThen); // bbThen->addInstruction(statement->getIR(bbThen));
        } else if (statement->shouldReturnABasicBlock()) {
            auto [then_begin, then_end] = statement->getBasicBlock(controlFlow);

            then_end->setExitWhenTrue(bbThen->getNextWhenTrue());
            // bbThen has no when_false

            bbThen->setExitWhenTrue(then_begin);
            // bbThen has no when_false

            bbThen = then_end->getNewWhenTrueBasicBlock("_thenafter");
        }
    }

    // NO ELSE BB
    if (mElseBlock.empty()) {
        bbCond->setExitWhenFalse(bbEnd);
        return {bbCond, bbEnd};
    }

    // ELSE BB
    bbCond->setExitWhenFalse(bbElse);
    bbElse->setExitWhenTrue(bbEnd);
    for (ast::Statement::Ptr const &statement : mElseBlock){
        if(statement->shouldReturnAnIR()) {
            SAFE_ADD_INSTRUCTION(statement, bbElse); // bbElse->addInstruction(statement->getIR(bbElse));
        } else if (statement->shouldReturnABasicBlock()){
            auto [else_begin, else_end] = statement->getBasicBlock(controlFlow);

            else_end->setExitWhenTrue(bbElse->getNextWhenTrue());
            // bbElse has no when_false

            bbElse->setExitWhenTrue(else_begin);
            // bbElse has no when_false

            bbElse = else_end->getNewWhenTrueBasicBlock("_elseafter");
        }
    }

    return {bbCond, bbEnd};


//    ir::IR::Ptr jumpInstructionElse;
//    if (ir::Operation::cmp_eq == conditionOperation) {
//        jumpInstructionElse = std::make_shared<ir::JumpEqualInstruction>(bb, bbElseName);
//    } else if (ir::Operation::cmp_lt == conditionOperation) {
//        jumpInstructionElse = std::make_shared<ir::JumpLessInstruction>(bb, bbElseName);
//    } else if (ir::Operation::cmp_le == conditionOperation) {
//        jumpInstructionElse = std::make_shared<ir::JumpLessOrEqualInstruction>(bb, bbElseName);
//    } else if (ir::Operation::cmp_gt == conditionOperation) {
//        jumpInstructionElse = std::make_shared<ir::JumpGreaterInstruction>(bb, bbElseName);
//    } else if (ir::Operation::cmp_ge == conditionOperation) {
//        jumpInstructionElse = std::make_shared<ir::JumpGreaterOrEqualInstruction>(bb, bbElseName);
//    } else {
//        jumpInstructionElse = std::make_shared<ir::JumpInstruction>(bb, bbElseName); // FIXME : Should not be the case
//    }
//    bb->addInstruction(jumpInstructionElse);
}

bool IfBlock::shouldReturnABasicBlock() const {
    return true;
}

} // namespace caramel::ast
