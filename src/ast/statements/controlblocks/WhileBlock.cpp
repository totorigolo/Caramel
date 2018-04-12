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

#include "WhileBlock.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/CFG.h"
#include "IfBlock.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

WhileBlock::WhileBlock(
        std::shared_ptr<caramel::ast::Expression> condition,
        std::vector<std::shared_ptr<caramel::ast::Statement>> block,
        antlr4::Token *token
) : ControlBlock(token, StatementType::WhileBlock),
    mCondition{std::move(condition)},
    mBlock{std::move(block)} {}

void WhileBlock::acceptAstDotVisit() {
    addNode(thisId(), "While");
    visitChildrenAstDot();
}

void WhileBlock::visitChildrenAstDot() {
    addEdge(thisId(), mCondition->thisId(), "condition");
    mCondition->acceptAstDotVisit();

    addNode(thisId() + 1, "Block");
    addEdge(thisId(), thisId() + 1);
    for (const auto &blockStatement : mBlock) {
        addEdge(thisId() + 1, blockStatement->thisId());
        blockStatement->acceptAstDotVisit();
    }
}

ir::GetBasicBlockReturn WhileBlock::getBasicBlock(
        ir::CFG *controlFlow
) {
    ir::BasicBlock::Ptr bbWcond = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Wcond");
    ir::BasicBlock::Ptr bbWthen = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Wthen");
    ir::BasicBlock::Ptr bbWend = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Wend");

    controlFlow->pushCurrentControlBlockEndBB(bbWend);

    bbWcond->setExitWhenTrue(bbWthen);
    bbWcond->setExitWhenFalse(bbWend);
    bbWthen->setExitWhenTrue(bbWcond);

    // COND BB
    if (mCondition->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(mCondition, bbWcond);
    } else if (mCondition->shouldReturnABasicBlock()) {
        logger.warning() << "Untested BB in while block condition BB.";

        auto[cond_begin, cond_end] = mCondition->getBasicBlock(controlFlow);

        bbWcond->setExitWhenTrue(cond_begin);
        // bb has no when_false
        cond_end->setExitWhenTrue(bbWthen);
        cond_end->setExitWhenFalse(bbWend); //< ?? Check mElseBlock.empty() before

        bbWcond = cond_end->getNewWhenTrueBasicBlock("_Wcondafter");
    }

    // THEN BB
    for (auto const &statement : mBlock) {
        if (statement->shouldReturnAnIR()) {
            SAFE_ADD_INSTRUCTION(statement, bbWthen);
        } else if (statement->shouldReturnABasicBlock()) {
            auto[then_begin, then_end] = statement->getBasicBlock(controlFlow);

            then_end->setExitWhenTrue(bbWthen->getNextWhenTrue());
            // bbThen has no when_false

            bbWthen->setExitWhenTrue(then_begin);
            // bbThen has no when_false

            bbWthen = then_end->getNewWhenTrueBasicBlock("_Wthenafter");
        }
    }

    controlFlow->popCurrentControlBlockEndBB();

    return {bbWcond, bbWend};
}

bool WhileBlock::shouldReturnABasicBlock() const {
    return true;
}

} // namespace caramel::ast
