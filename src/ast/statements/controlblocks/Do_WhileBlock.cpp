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

#include "Do_WhileBlock.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/CFG.h"
#include "IfBlock.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

Do_WhileBlock::Do_WhileBlock(
        std::shared_ptr<caramel::ast::Expression> condition,
        std::vector<std::shared_ptr<caramel::ast::Statement>> block,
        antlr4::Token *token
) : ControlBlock(token, StatementType::DoWhileBlock),
    mCondition{std::move(condition)},
    mBlock{std::move(block)} {}

void Do_WhileBlock::acceptAstDotVisit() {
    addNode(thisId(), "While");
    visitChildrenAstDot();
}

void Do_WhileBlock::visitChildrenAstDot() {
    addEdge(thisId(), mCondition->thisId(), "condition");
    mCondition->acceptAstDotVisit();

    addNode(thisId() + 1, "Block");
    addEdge(thisId(), thisId() + 1);
    for (const auto &blockStatement : mBlock) {
        addEdge(thisId() + 1, blockStatement->thisId());
        blockStatement->acceptAstDotVisit();
    }
}

ir::GetBasicBlockReturn Do_WhileBlock::getBasicBlock(
        ir::CFG *controlFlow
) {
    ir::BasicBlock::Ptr bbDWcond = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_DWcond");
    ir::BasicBlock::Ptr bbDWaction = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_DWaction");
    ir::BasicBlock::Ptr bbDWend = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_DWend");

    controlFlow->pushCurrentControlBlockEndBB(bbDWend);

    bbDWaction->setExitWhenTrue(bbDWcond);
    bbDWcond->setExitWhenTrue(bbDWaction);
    bbDWcond->setExitWhenFalse(bbDWend);

    // COND BB
    if (mCondition->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(mCondition, bbDWcond); // bbDWcond->addInstruction(mCondition->getIR(bbDWcond));
    } else if (mCondition->shouldReturnABasicBlock()) {
        logger.warning() << "Untested BB in while block condition BB.";

        auto[cond_begin, cond_end] = mCondition->getBasicBlock(controlFlow);

        bbDWcond->setExitWhenTrue(cond_begin);
        // bb has no when_false
        cond_end->setExitWhenTrue(bbDWaction);
        cond_end->setExitWhenFalse(bbDWend); //< ?? Check mElseBlock.empty() before

        bbDWcond = cond_end->getNewWhenTrueBasicBlock("_DWcondafter");
    }

    // Action BB
    for (auto const &statement : mBlock) {
        if (statement->shouldReturnAnIR()) {
            SAFE_ADD_INSTRUCTION(statement, bbDWaction); // bbDWaction->addInstruction(statement->getIR(bbDWaction));
        } else if (statement->shouldReturnABasicBlock()) {
            auto[then_begin, then_end] = statement->getBasicBlock(controlFlow);

            then_end->setExitWhenTrue(bbDWaction->getNextWhenTrue());
            // bbThen has no when_false

            bbDWaction->setExitWhenTrue(then_begin);
            // bbThen has no when_false

            bbDWaction = then_end->getNewWhenTrueBasicBlock("_DWthenafter");
        }
    }

    controlFlow->popCurrentControlBlockEndBB();

    return {bbDWaction, bbDWend};
}

bool Do_WhileBlock::shouldReturnABasicBlock() const {
    return true;
}

} // namespace caramel::ast
