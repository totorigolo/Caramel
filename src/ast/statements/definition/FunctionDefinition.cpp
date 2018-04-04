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

std::shared_ptr<ir::BasicBlock> FunctionDefinition::getBasicBlock(
        ir::CFG *controlFlow
) {
    ir::BasicBlock::Ptr bb = controlFlow->generateFunctionBlock(mSymbol->getName());

    controlFlow->enterFunction(bb->getId());
    bb->addInstruction(std::make_shared<ir::PrologInstruction>(bb, 42)); // FIXME: Function definition IR

    for (ast::Statement::Ptr const &statement : mContext->getStatements()) {
        if (statement->shouldReturnAnIR()) {
            bb->addInstruction(statement->getIR(bb));
        } else if (statement->shouldReturnABasicBlock()) {
            controlFlow->addBasicBlock(statement->getBasicBlock(controlFlow));
        }
    }

    bb->addInstruction(std::make_shared<ir::EpilogInstruction>(bb));
    controlFlow->leaveFunction(bb->getId());

    return bb;
}

} // namespace caramel::ast
