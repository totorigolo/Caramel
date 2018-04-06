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

#include "ArrayDefinition.h"

#include "../../symboltable/ArraySymbol.h"
#include "../../../utils/Common.h"
#include "../expressions/atomicexpression/Constant.h"
#include "../../../ir/BasicBlock.h"
#include "../../../ir/instructions/CopyInstruction.h"
#include "../../../ir/instructions/NopInstruction.h"
#include "../../../ir/helpers/IROperatorHelper.h"


namespace caramel::ast {

ArrayDefinition::ArrayDefinition(antlr4::Token *startToken)
        : Definition(startToken, StatementType::ArrayDefinition),
          mStartToken{startToken}, mSymbol{} {}

std::weak_ptr<caramel::ast::Symbol> ArrayDefinition::getSymbol() {
    return mSymbol;
}

void ArrayDefinition::setSymbol(std::shared_ptr<ArraySymbol> const &symbol) {
    mSymbol = symbol;
}

void ArrayDefinition::acceptAstDotVisit() {
    using namespace caramel::utils;
    if (!mSymbol.lock()) {
        addErrorNode(thisId(), "ArrayDefinition", "ArraySymbol is null.");
        return;
    }
    auto arraySymbol = castTo<ArraySymbol::Ptr>(mSymbol.lock());
    addNode(thisId(), "ArrayDefinition: " + arraySymbol->getName());
    addEdge(thisId(), arraySymbol->thisId());
}

bool ArrayDefinition::shouldReturnAnIR() const {
    return true;
}

std::shared_ptr<ir::IR> ArrayDefinition::getIR(ir::BasicBlock::Ptr &currentBasicBlock) {

    auto arraySymbol = mSymbol.lock();
    auto arrayName = arraySymbol->getName();
    auto const &content = arraySymbol->getContent();

    long index = arraySymbol->getSize();
    for (auto it = content.rbegin(), it_end = content.rend(); it != it_end; ++it) {
        auto const &expression = *it;
        --index;

        std::string valueName = SAFE_ADD_INSTRUCTION(expression, currentBasicBlock); // currentBasicBlock->addInstruction(expression->getIR(currentBasicBlock));
        std::string destination = arrayName;
        if (index > 0) {
            destination += "[" + std::to_string(index) + "]";
        }
        currentBasicBlock->addInstruction(std::make_shared<ir::CopyInstruction>(
                currentBasicBlock,
                arraySymbol->getType(),
                destination,
                valueName
        ));
    }

    return std::make_shared<ir::NopInstruction>(currentBasicBlock);
}

} // namespace caramel::ast
