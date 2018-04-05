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

#include "FunctionDefinition.h"
#include "../../../ir/CFG.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/IR.h"
#include "../../../ir/instructions/PrologInstruction.h"
#include "../../../ir/instructions/EpilogInstruction.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/PushInstruction.h"


namespace caramel::ast {

FunctionDefinition::FunctionDefinition(
        std::shared_ptr<Context> context,
        antlr4::Token *startToken
) : Definition(startToken, StatementType::FunctionDefinition), mContext{std::move(context)}, mSymbol{} {}

std::weak_ptr<Symbol> FunctionDefinition::getSymbol() {
    return mSymbol;
}

void FunctionDefinition::setSymbol(FunctionSymbol::Ptr functionSymbol) {
    mSymbol = std::move(functionSymbol);
}

void FunctionDefinition::acceptAstDotVisit() {
    addNode(thisId(), "FunctionDefinition: " + mSymbol->getName());
    visitChildrenAstDot();
}

void FunctionDefinition::visitChildrenAstDot() {
    addEdge(thisId(), mContext->thisId());
    mContext->acceptAstDotVisit();
    addEdge(thisId(), mSymbol->thisId());
}

ir::GetBasicBlockReturn FunctionDefinition::getBasicBlock(
        ir::CFG *controlFlow
) {
    ir::BasicBlock::Ptr function_root_bb = controlFlow->generateFunctionBlock(mSymbol->getName());

    controlFlow->enterFunction(function_root_bb->getId());
    function_root_bb->addInstruction(
            std::make_shared<ir::PrologInstruction>(function_root_bb, 42)); // FIXME: Function definition IR

    auto parameters = mSymbol->getParameters();
    for (size_t i = 0; i < parameters.size(); i++) {
        if (i < 6) {
            function_root_bb->addInstruction(
                    std::make_shared<ir::CopyInstruction>(function_root_bb, parameters[i].primaryType,
                                                          parameters[i].name, i)
            );
        } else {
            function_root_bb->addSymbol(parameters[i].name, Int64_t::Create(), 16 + i * 8);
        }

    }


    ir::BasicBlock::Ptr function_end_bb = controlFlow->generateBasicBlock(ir::BasicBlock::getNextNumberName() + "_endof_" + mSymbol->getName());
    function_root_bb->setExitWhenTrue(function_end_bb);

    controlFlow->addFunctionBBEnd(function_root_bb->getFunctionContext(),function_end_bb);

    ir::BasicBlock::Ptr bb = function_root_bb->getNewWhenTrueBasicBlock("_innerbeginof_" + mSymbol->getName());
    for (ast::Statement::Ptr const &statement : mContext->getStatements()) {
        if (statement->shouldReturnAnIR()) {
            auto ir = statement->getIR(bb);
            bb->addInstruction(ir);
        } else if (statement->shouldReturnABasicBlock()) {
            auto [child_root, child_end] = statement->getBasicBlock(controlFlow);

            if (!bb->getNextWhenTrue()) {
                logger.fatal() << "BB must have a child.";
                exit(1);
            }
            bb->setExitWhenTrue(child_root);
            // bb has no when_false
//            child_end->setExitWhenTrue(bb->getNextWhenTrue());
            child_end->setExitWhenTrue(function_end_bb);

            bb = child_end->getNewWhenTrueBasicBlock("_funcinnerafter");
        }
    }

    function_end_bb->addInstruction(std::make_shared<ir::EpilogInstruction>(function_root_bb));
    controlFlow->leaveFunction(function_root_bb->getId());

    return {function_root_bb, function_end_bb};
}

} // namespace caramel::ast
