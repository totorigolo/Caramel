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

#include "FunctionCall.h"
#include "../../../../utils/Common.h"
#include "../../../symboltable/Symbol.h"
#include "../../../../ir/BasicBlock.h"
#include "../../../../ir/instructions/CallParameterInstruction.h"
#include "../../../../ir/instructions/FunctionCallInstruction.h"


namespace caramel::ast {

using namespace utils;

FunctionCall::FunctionCall(std::vector<Expression::Ptr> &&arguments, antlr4::Token *startToken)
        : AtomicExpression(startToken, StatementType::FunctionCall), mArguments{arguments} {
}

Symbol::Ptr FunctionCall::getSymbol() const {
    return mSymbol;
}

void FunctionCall::setSymbol(FunctionSymbol::Ptr symbol) {
    mSymbol = std::move(symbol);
}

PrimaryType::Ptr FunctionCall::getPrimaryType() const {
    return mSymbol->getType();
}

std::vector<PrimaryType::Ptr> FunctionCall::getArgumentsPrimaryTypes() const {
    std::vector<PrimaryType::Ptr> argumentsTypes;
    for (auto const& argument : mArguments) {
        argumentsTypes.push_back(argument->getPrimaryType());
    }
    return argumentsTypes;
}

void FunctionCall::acceptAstDotVisit() {
    addNode(thisId(), "FunctionCall: " + mSymbol->getName());
    addEdge(thisId(), mSymbol->thisId());
    mSymbol->acceptAstDotVisit();
    visitChildrenAstDot();
}

void FunctionCall::visitChildrenAstDot() {
    for (auto const& argument : mArguments) {
        addEdge(thisId(), argument->thisId());
        argument->acceptAstDotVisit();
    }
}

std::shared_ptr<ir::IR> FunctionCall::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock) {

    std::string functionName = mSymbol->getName();
    auto functionSymbol = castTo<FunctionSymbol::Ptr>(mSymbol);

    std::vector<std::string> arguments;
    for(int i = mArguments.size() - 1; i >= 0; i--) {
        std::string tempVar = currentBasicBlock->addInstruction((mArguments[i])->getIR(currentBasicBlock));
        currentBasicBlock->addInstruction(
                std::make_shared<ir::CallParameterInstruction>(currentBasicBlock, i, mArguments[i]->getPrimaryType(), tempVar)
        );
    }

    return std::make_shared<ir::FunctionCallInstruction>(
            functionName,
            currentBasicBlock,
            functionSymbol->getType(),
            mArguments.size()
    );
}

} // namespace caramel::ast
