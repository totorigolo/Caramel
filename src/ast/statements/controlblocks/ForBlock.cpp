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

#include "ForBlock.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

ForBlock::ForBlock(
        std::shared_ptr<caramel::ast::Expression> begin,
        std::shared_ptr<caramel::ast::Expression> end,
        std::shared_ptr<caramel::ast::Expression> step,
        std::vector<std::shared_ptr<caramel::ast::Statement>> block,
        antlr4::Token *token
) : ControlBlock(token), mBegin{std::move(begin)}, mEnd{std::move(end)}, mStep{std::move(step)},
    mBlock{std::move(block)} {}

void ForBlock::acceptAstDotVisit() {
    addNode(thisId(), "For");
    visitChildrenAstDot();
}

void ForBlock::visitChildrenAstDot() {
    addNode(thisId() + 1, "Condition");
    addEdge(thisId(), thisId() + 1);

    addEdge(thisId() + 1, mBegin->thisId(), "begin");
    mBegin->acceptAstDotVisit();

    addEdge(thisId() + 1, mEnd->thisId(), "end");
    mEnd->acceptAstDotVisit();

    addEdge(thisId() + 1, mStep->thisId(), "step");
    mStep->acceptAstDotVisit();

    addNode(thisId() + 2, "Block");
    addEdge(thisId(), thisId() + 2);
    for (const auto &blockStatement : mBlock) {
        addEdge(thisId() + 2, blockStatement->thisId());
        blockStatement->acceptAstDotVisit();
    }
}

bool ForBlock::shouldReturnABasicBlock() const {
    return true;
}

ir::GetBasicBlockReturn ForBlock::getBasicBlock(ir::CFG *controlFlow) {

    ir::BasicBlock::Ptr bbInit = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Finit");
    ir::BasicBlock::Ptr bbCond = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Fcond");
    ir::BasicBlock::Ptr bbThen = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Fthen");
    ir::BasicBlock::Ptr bbInc = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Finc");
    ir::BasicBlock::Ptr bbEnd = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_Fend");

    bbInit->setExitWhenTrue(bbCond);
    bbCond->setExitWhenTrue(bbThen);
    bbCond->setExitWhenFalse(bbEnd);
    bbThen->setExitWhenTrue(bbInc);
    bbInc->setExitWhenTrue(bbCond);

    // INIT BB
    if (mBegin->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(mBegin, bbInit); // bbInit->addInstruction(mBegin->getIR(bbInit));
    } else if (mBegin->shouldReturnABasicBlock()) {
        auto [init_begin, init_end] = mBegin->getBasicBlock(controlFlow);

        init_end->setExitWhenTrue(bbInit->getNextWhenTrue());
        // bbInit has no when_false

        bbInit->setExitWhenTrue(init_begin);
        // bbInit has no when_false

        bbInit = init_end->getNewWhenTrueBasicBlock("_Finitafter");
    }


    // COND BB
    if (mEnd->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(mEnd, bbCond); // bbCond->addInstruction(mEnd->getIR(bbCond));
    } else if (mEnd->shouldReturnABasicBlock()) {
        logger.warning() << "Untested BB in for block condition BB.";

        auto [cond_begin, cond_end] = mBegin->getBasicBlock(controlFlow);

        bbCond->setExitWhenTrue(cond_begin);
        // bb has no when_false
        cond_end->setExitWhenTrue(bbThen);
        cond_end->setExitWhenFalse(bbEnd);

        bbCond = cond_end->getNewWhenTrueBasicBlock("_Fcondafter");
    }

    // THEN BB
    for (auto const &statement : mBlock) {
        if (statement->shouldReturnAnIR()) {
            SAFE_ADD_INSTRUCTION(statement, bbThen); // bbThen->addInstruction(statement->getIR(bbThen));
        } else if (statement->shouldReturnABasicBlock()) {
            auto [then_begin, then_end] = statement->getBasicBlock(controlFlow);

            then_end->setExitWhenTrue(bbThen->getNextWhenTrue());
            // bbThen has no when_false

            bbThen->setExitWhenTrue(then_begin);
            // bbThen has no when_false

            bbThen = then_end->getNewWhenTrueBasicBlock("_Fthenafter");
        }
    }


    // INC BB
    if (mStep->shouldReturnAnIR()) {
        SAFE_ADD_INSTRUCTION(mStep, bbInc); // bbInc->addInstruction(mStep->getIR(bbInc));
    } else if (mStep->shouldReturnABasicBlock()) {
        auto [inc_begin, inc_end] = mStep->getBasicBlock(controlFlow);

        inc_end->setExitWhenTrue(bbInc->getNextWhenTrue());
        // bbInc has no when_false

        bbInc->setExitWhenTrue(inc_begin);
        // bbInc has no when_false

        bbInc = inc_end->getNewWhenTrueBasicBlock("_Fincafter");
    }


    return {bbInit, bbEnd};

}


} // namespace caramel::ast
