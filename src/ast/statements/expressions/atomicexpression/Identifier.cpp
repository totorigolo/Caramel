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

#include "Identifier.h"
#include "../../../../ir/BasicBlock.h"
#include "../../../../ir/instructions/EmptyInstruction.h"
#include "../../../../ir/instructions/NopInstruction.h"

namespace caramel::ast {

Identifier::Identifier(antlr4::Token *startToken) :
        LValue(startToken, StatementType::Identifier), mSymbol{} {}

Symbol::Ptr Identifier::getSymbol() const {
    return mSymbol;
}

void Identifier::setSymbol(VariableSymbol::Ptr symbol) {
    mSymbol = std::move(symbol);
}

PrimaryType::Ptr Identifier::getPrimaryType() const {
    return mSymbol->getType();
}

std::shared_ptr<ir::IR> Identifier::getIR(std::shared_ptr<ir::BasicBlock> &currentBasicBlock) {
    return std::make_shared<ir::EmptyInstruction>(mSymbol->getName(), currentBasicBlock, mSymbol->getType());
}

bool Identifier::shouldReturnAnIR() const {
    return true;
}

void Identifier::acceptAstDotVisit() {
    addNode(thisId(), "Identifier: " + mSymbol->getName());
    addEdge(thisId(), mSymbol->thisId());
}

} // namespace caramel::ast
