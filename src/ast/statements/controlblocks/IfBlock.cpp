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
#include "../../../ir/instructions/JumpEqualInstruction.h"
#include "../../../ir/instructions/JumpLessInstruction.h"
#include "../../../ir/instructions/JumpLessOrEqualInstruction.h"
#include "../../../ir/instructions/JumpInstruction.h"
#include "../../../ir/instructions/JumpGreaterInstruction.h"
#include "../../../ir/instructions/JumpGreaterOrEqualInstruction.h"


namespace caramel::ast {

IfBlock::IfBlock(
        std::shared_ptr<caramel::ast::Expression> const &condition,
        std::vector<std::shared_ptr<caramel::ast::Statement>> const &thenBlock,
        std::vector<std::shared_ptr<caramel::ast::Statement>> const &elseBlock,
        antlr4::Token *startToken
) : ControlBlock(startToken), mCondition{condition}, mThenBlock{thenBlock}, mElseBlock{elseBlock} {
}

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

    addNode(thisId() + 2, "Else block");
    addEdge(thisId(), thisId() + 2);
    for (const auto &elseStatement : mElseBlock) {
        addEdge(thisId() + 2, elseStatement->thisId());
        elseStatement->acceptAstDotVisit();
    }
}



std::shared_ptr<ir::BasicBlock> IfBlock::getBasicBlock(
        ir::CFG *controlFlow
) {
    caramel::ir::BasicBlock::Ptr bb = controlFlow->generateBasicBlock();
    bb->mIsControlBlock = true;

    std::string bbEndName = caramel::ir::BasicBlock::getNextNumberName();
    caramel::ir::BasicBlock::Ptr bbEnd = controlFlow->generateBasicBlock(bbEndName);

    if (mCondition->shouldReturnAnIR()) {
        bb->addInstruction(mCondition->getIR(bb));
    } else if (mCondition->shouldReturnABasicBlock()) {
        bb->addInstructions(mCondition->getBasicBlock(controlFlow));
    }

//    ir::Operation conditionOperation = bb->getInstructions().back()->getOperation();


    std::string bbElseName = caramel::ir::BasicBlock::getNextNumberName();

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


    std::string bbThenName = caramel::ir::BasicBlock::getNextNumberName();
    caramel::ir::BasicBlock::Ptr bbThen = controlFlow->generateBasicBlock(bbThenName);
    bbThen->mIsControlBlock = true;



    for(caramel::ast::Statement::Ptr const &statement : mThenBlock){
        if(statement->shouldReturnAnIR()) {
            bbThen->addInstruction(statement->getIR(bbThen));
        } else if (statement->shouldReturnABasicBlock()){
            ir::BasicBlock::Ptr bbE = statement->getBasicBlock(controlFlow);
            if(bbE->mIsControlBlock) {
                bbThen = bbE;
                bbThen->setLabelName(bbThenName);



            } else {
                bbThen->addInstructions(bbE);
                bbThen->setMExitWhenTrue(bbEnd);
            }
        }
    }

    bb->setMExitWhenTrue(bbThen);



//    ir::IR::Ptr jumpInstructionEnd = std::make_shared<ir::JumpInstruction>(bb, bbEndName);
//    bb->addInstruction(jumpInstructionEnd);

    caramel::ir::BasicBlock::Ptr bbElse = controlFlow->generateBasicBlock(bbElseName);
    bbElse->mIsControlBlock = true;




    if(mElseBlock.empty()) {

        bb->setMExitWhenFalse(bbEnd);

    } else {
        for(caramel::ast::Statement::Ptr const &statement : mElseBlock){
            if(statement->shouldReturnAnIR()) {
                bbElse->addInstruction(statement->getIR(bbElse));
            } else if (statement->shouldReturnABasicBlock()){
                ir::BasicBlock::Ptr bbE = statement->getBasicBlock(controlFlow);
                if(bbE->mIsControlBlock) {
                    bbElse = bbE;
                    bbElse->setLabelName(bbElseName);
                } else {
                    bbElse->addInstructions(bbE);
                    bbElse->setMExitWhenTrue(bbEnd);
                }

            }
        }

        bb->setMExitWhenFalse(bbElse);
    }


 //   controlFlow->addBasicBlock(bb);

    //bbElse->addInstruction(jumpInstructionEnd);


    return bb;
}

bool IfBlock::shouldReturnABasicBlock() const {
    return true;
}

} // namespace caramel::ast
